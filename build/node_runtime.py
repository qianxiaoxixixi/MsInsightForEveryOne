"""Stage a pinned, minimal Node.js runtime for the product (never npm/npx)."""

import hashlib
import json
import logging
import os
import platform
import shutil
import stat
import tarfile
import tempfile
import urllib.request
import zipfile
from pathlib import Path


NODE_VERSION = '22.23.5'
# Reviewed identities from https://nodejs.org/dist/v22.23.5/SHASUMS256.txt.
ARCHIVE_SHA256 = {
    ('win32', 'x64'): '4a191082e9706866830379d577d2cc8a50c776fcd9436a8d061b063ed0d9a0e3',
    ('win32', 'arm64'): 'c1856817f611738c77534efbff8a5a17990593eab24046bc8d2c23a77a160d13',
    ('linux', 'x64'): 'cdeb8670a0c81f51b4dc4ac61919f9b1ad917b304bf8221eb939570663d240f8',
    ('linux', 'arm64'): '2babb9eb5264860d540849f4b064a6fbdebcdc323657d9d382ce23d313fd5e09',
    ('darwin', 'x64'): 'c53383238206b9ae4db23c5d8a63645e2e753bec8ab51a49cc81939aa64e9422',
    ('darwin', 'arm64'): 'e7131995759b61a0170b646010d2ad35a5d37b4cb0d471f3e8317eac6f742f3d',
}


def runtime_target(env=None):
    env = os.environ if env is None else env
    os_name = env.get('MSINSIGHT_RAG_TARGET_PLATFORM', platform.system()).lower()
    architecture = env.get('MSINSIGHT_RAG_TARGET_ARCH', platform.machine()).lower()
    os_name = {'windows': 'win32'}.get(os_name, os_name)
    architecture = {'amd64': 'x64', 'x86_64': 'x64', 'aarch64': 'arm64'}.get(architecture, architecture)
    if (os_name, architecture) not in ARCHIVE_SHA256:
        raise ValueError(f'Unsupported bundled Node.js target: {os_name}/{architecture}')
    if os_name == 'linux' and env.get('MSINSIGHT_RAG_TARGET_LIBC', 'glibc') != 'glibc':
        raise ValueError('The bundled Linux Node.js runtime requires glibc')
    return os_name, architecture


def archive_name(target):
    os_name, architecture = target
    suffix = {'win32': 'zip', 'linux': 'tar.xz', 'darwin': 'tar.gz'}[os_name]
    distribution_os = 'win' if os_name == 'win32' else os_name
    return f'node-v{NODE_VERSION}-{distribution_os}-{architecture}.{suffix}'


def file_sha256(path):
    digest = hashlib.sha256()
    with Path(path).open('rb') as stream:
        for chunk in iter(lambda: stream.read(1024 * 1024), b''):
            digest.update(chunk)
    return digest.hexdigest()


def verified_archive(cache_dir, target, offline):
    cache_dir = Path(cache_dir)
    archive = cache_dir / archive_name(target)
    if archive.exists():
        if archive.is_symlink() or not archive.is_file() or file_sha256(archive) != ARCHIVE_SHA256[target]:
            raise ValueError(f'Bundled Node.js archive failed SHA-256 verification: {archive}')
        logging.info('Using verified cached Node.js runtime: %s', archive)
        return archive
    if offline:
        raise FileNotFoundError(f'Offline build requires the verified Node.js archive: {archive}')
    cache_dir.mkdir(parents=True, exist_ok=True)
    url = f'https://nodejs.org/dist/v{NODE_VERSION}/{archive.name}'
    logging.info('Downloading pinned Node.js runtime: %s', url)
    with tempfile.TemporaryDirectory(prefix='.node-download-', dir=cache_dir) as temporary:
        download = Path(temporary) / archive.name
        with urllib.request.urlopen(url, timeout=120) as response, download.open('wb') as output:
            shutil.copyfileobj(response, output)
        if file_sha256(download) != ARCHIVE_SHA256[target]:
            raise ValueError(f'Downloaded Node.js archive failed SHA-256 verification: {url}')
        os.replace(download, archive)
    logging.info('Cached verified Node.js runtime: %s', archive)
    return archive


def extract_runtime(archive, destination, target):
    """Copy only exact, regular-file members; never extract arbitrary archive paths."""
    os_name, architecture = target
    distribution_os = 'win' if os_name == 'win32' else os_name
    root = f'node-v{NODE_VERSION}-{distribution_os}-{architecture}'
    executable = 'node.exe' if os_name == 'win32' else 'node'
    member = executable if os_name == 'win32' else 'bin/node'
    wanted = {f'{root}/{member}': executable, f'{root}/LICENSE': 'LICENSE'}
    if os_name == 'win32':
        with zipfile.ZipFile(archive) as package:
            for source, name in wanted.items():
                matches = [info for info in package.infolist() if info.filename == source]
                if (
                    len(matches) != 1
                    or matches[0].is_dir()
                    or matches[0].file_size <= 0
                    or stat.S_IFMT(matches[0].external_attr >> 16) not in (0, stat.S_IFREG)
                ):
                    raise ValueError(f'Invalid Node.js archive member: {source}')
                with package.open(matches[0]) as stream, (destination / name).open('wb') as output:
                    shutil.copyfileobj(stream, output)
    else:
        with tarfile.open(archive, 'r:*') as package:
            for source, name in wanted.items():
                matches = [info for info in package.getmembers() if info.name == source]
                if len(matches) != 1 or not matches[0].isfile() or matches[0].size <= 0:
                    raise ValueError(f'Invalid Node.js archive member: {source}')
                with package.extractfile(matches[0]) as stream, (destination / name).open('wb') as output:
                    shutil.copyfileobj(stream, output)
    # Windows interprets these modes as the read-only attribute, which would
    # prevent the next build from cleaning its intermediate output directory.
    if os.name != 'nt':
        (destination / executable).chmod(0o550)
        (destination / 'LICENSE').chmod(0o440)


def stage_node_runtime(bundle_dir, cache_dir, *, offline=False, env=None):
    target = runtime_target(env)
    archive = verified_archive(cache_dir, target, offline)
    bundle_dir = Path(bundle_dir)
    runtime = bundle_dir / 'runtime'
    # A fresh server bundle must not silently retain an unrelated runtime.
    if runtime.exists():
        raise ValueError(f'Node.js runtime output already exists: {runtime}')
    with tempfile.TemporaryDirectory(prefix='.node-runtime-', dir=bundle_dir) as temporary:
        # The cache may be shared with other builds. Pin this build's bytes in a
        # private copy, verify that copy, and never reopen the cache for extraction.
        private_archive = Path(temporary) / archive.name
        shutil.copyfile(archive, private_archive)
        if file_sha256(private_archive) != ARCHIVE_SHA256[target]:
            raise ValueError(f'Bundled Node.js archive failed SHA-256 verification: {archive}')
        staging = Path(temporary) / 'runtime'
        staging.mkdir()
        try:
            extract_runtime(private_archive, staging, target)
        except (tarfile.TarError, zipfile.BadZipFile, KeyError) as error:
            raise ValueError(f'Invalid Node.js runtime archive: {archive}') from error
        metadata = {
            'version': NODE_VERSION,
            'platform': target[0],
            'arch': target[1],
            'archive': archive.name,
            'archiveSha256': ARCHIVE_SHA256[target],
        }
        (staging / 'runtime.json').write_text(json.dumps(metadata, indent=2) + '\n', encoding='utf-8')
        os.replace(staging, runtime)
    logging.info('Bundled Node.js %s (%s/%s) at %s', NODE_VERSION, *target, runtime)
