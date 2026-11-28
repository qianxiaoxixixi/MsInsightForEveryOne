"""Minimal-runtime packaging contracts; no real download or product build."""

import hashlib
import importlib.util
import io
import json
import os
import stat
import sys
import tarfile
import zipfile
from pathlib import Path
from types import SimpleNamespace

import pytest

BUILD_DIR = Path(__file__).parents[2]
if str(BUILD_DIR) not in sys.path:
    sys.path.insert(0, str(BUILD_DIR))

import node_runtime as runtime  # noqa: E402 - load the build helper after adding its directory


def load_build_module():
    spec = importlib.util.spec_from_file_location('node_packaging_build_test', BUILD_DIR / 'build.py')
    build = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(build)
    return build


def make_archive(directory, target, *, symlink=False, duplicate=False):
    archive = directory / runtime.archive_name(target)
    distribution_os = 'win' if target[0] == 'win32' else target[0]
    root = f'node-v{runtime.NODE_VERSION}-{distribution_os}-{target[1]}'
    executable = 'node.exe' if target[0] == 'win32' else 'bin/node'
    entries = {executable: b'test runtime', 'LICENSE': b'test license', 'lib/node_modules/npm/index.js': b'excluded'}
    if target[0] == 'win32':
        with zipfile.ZipFile(archive, 'w') as package:
            for name, content in entries.items():
                info = zipfile.ZipInfo(f'{root}/{name}')
                if symlink and name == executable:
                    info.create_system = 3
                    info.external_attr = (stat.S_IFLNK | 0o777) << 16
                package.writestr(info, content)
                if duplicate and name == executable:
                    with pytest.warns(UserWarning, match='Duplicate name'):
                        package.writestr(info, content)
    else:
        mode = 'w:xz' if target[0] == 'linux' else 'w:gz'
        with tarfile.open(archive, mode) as package:
            for name, content in entries.items():
                info = tarfile.TarInfo(f'{root}/{name}')
                info.size = len(content)
                if symlink and name == executable:
                    info.type = tarfile.SYMTYPE
                    info.linkname = '../../outside'
                    info.size = 0
                package.addfile(info, io.BytesIO(content))
                if duplicate and name == executable:
                    package.addfile(info, io.BytesIO(content))
    return archive


@pytest.mark.parametrize('target', list(runtime.ARCHIVE_SHA256))
def test_stages_only_target_executable_license_and_identity(tmp_path, monkeypatch, target):
    cache = tmp_path / 'cache'
    cache.mkdir()
    archive = make_archive(cache, target)
    monkeypatch.setitem(runtime.ARCHIVE_SHA256, target, runtime.file_sha256(archive))
    monkeypatch.setattr(runtime.urllib.request, 'urlopen', lambda *_args, **_kwargs: pytest.fail('offline'))
    bundle = tmp_path / '安装目录 with spaces'
    bundle.mkdir()
    environment = {'MSINSIGHT_RAG_TARGET_PLATFORM': target[0], 'MSINSIGHT_RAG_TARGET_ARCH': target[1]}

    runtime.stage_node_runtime(bundle, cache, offline=True, env=environment)

    executable = 'node.exe' if target[0] == 'win32' else 'node'
    assert {entry.name for entry in (bundle / 'runtime').iterdir()} == {executable, 'LICENSE', 'runtime.json'}
    assert (bundle / 'runtime' / executable).read_bytes() == b'test runtime'
    metadata = json.loads((bundle / 'runtime' / 'runtime.json').read_text(encoding='utf-8'))
    assert (metadata['platform'], metadata['arch']) == target
    assert metadata['archiveSha256'] == runtime.file_sha256(archive)
    if os.name != 'nt':
        assert os.access(bundle / 'runtime' / executable, os.X_OK)
    else:
        assert not (bundle / 'runtime' / executable).stat().st_file_attributes & stat.FILE_ATTRIBUTE_READONLY
        assert not (bundle / 'runtime' / 'LICENSE').stat().st_file_attributes & stat.FILE_ATTRIBUTE_READONLY


@pytest.mark.parametrize('target', [('win32', 'x64'), ('linux', 'arm64')])
@pytest.mark.parametrize('invalid', ['symlink', 'duplicate'])
def test_refuses_links_and_duplicate_executable_members(tmp_path, target, invalid):
    archive = make_archive(tmp_path, target, **{invalid: True})
    destination = tmp_path / 'runtime'
    destination.mkdir()
    with pytest.raises(ValueError, match='Invalid Node.js archive member'):
        runtime.extract_runtime(archive, destination, target)


@pytest.mark.parametrize('target', list(runtime.ARCHIVE_SHA256))
@pytest.mark.parametrize('mutation', ['before-copy', 'during-copy', 'after-verification'])
def test_shared_cache_changes_cannot_publish_unverified_bytes(tmp_path, monkeypatch, target, mutation):
    cache = tmp_path / 'cache'
    cache.mkdir()
    archive = make_archive(cache, target)
    expected_digest = runtime.file_sha256(archive)
    monkeypatch.setitem(runtime.ARCHIVE_SHA256, target, expected_digest)
    bundle = tmp_path / 'bundle'
    bundle.mkdir()
    original_verify = runtime.verified_archive
    original_copy = runtime.shutil.copyfile
    original_hash = runtime.file_sha256
    original_extract = runtime.extract_runtime
    extracted = []

    def verify(*args):
        result = original_verify(*args)
        if mutation == 'before-copy':
            archive.write_bytes(b'replaced after cache verification')
        return result

    def copy(source, destination):
        result = original_copy(source, destination)
        if mutation == 'during-copy':
            # Model a shared archive changing while its bytes are being copied.
            with Path(destination).open('ab') as output:
                output.write(b'concurrent cache write')
        return result

    def digest(path):
        result = original_hash(path)
        if Path(path) != archive and mutation == 'after-verification':
            archive.write_bytes(b'replaced before extraction')
        return result

    def extract(source, destination, selected_target):
        assert source != archive
        assert source.parent.parent == bundle
        assert original_hash(source) == expected_digest
        extracted.append(source)
        return original_extract(source, destination, selected_target)

    monkeypatch.setattr(runtime, 'verified_archive', verify)
    monkeypatch.setattr(runtime.shutil, 'copyfile', copy)
    monkeypatch.setattr(runtime, 'file_sha256', digest)
    monkeypatch.setattr(runtime, 'extract_runtime', extract)
    environment = {'MSINSIGHT_RAG_TARGET_PLATFORM': target[0], 'MSINSIGHT_RAG_TARGET_ARCH': target[1]}

    if mutation == 'after-verification':
        runtime.stage_node_runtime(bundle, cache, offline=True, env=environment)
        executable = 'node.exe' if target[0] == 'win32' else 'node'
        assert (bundle / 'runtime' / executable).read_bytes() == b'test runtime'
        metadata = json.loads((bundle / 'runtime' / 'runtime.json').read_text(encoding='utf-8'))
        assert metadata['archiveSha256'] == expected_digest
        assert len(extracted) == 1
        assert not extracted[0].exists()
        assert {entry.name for entry in bundle.iterdir()} == {'runtime'}
    else:
        with pytest.raises(ValueError, match='SHA-256'):
            runtime.stage_node_runtime(bundle, cache, offline=True, env=environment)
        assert not extracted
        assert not list(bundle.iterdir())


def test_offline_missing_archive_fails_without_network(tmp_path, monkeypatch):
    monkeypatch.setattr(runtime.urllib.request, 'urlopen', lambda *_args, **_kwargs: pytest.fail('network'))
    with pytest.raises(FileNotFoundError, match='Offline build requires'):
        runtime.verified_archive(tmp_path, ('win32', 'x64'), True)


def test_partial_extraction_does_not_publish_a_runtime(tmp_path, monkeypatch):
    target = ('win32', 'x64')
    archive = tmp_path / runtime.archive_name(target)
    with zipfile.ZipFile(archive, 'w') as package:
        package.writestr(f'node-v{runtime.NODE_VERSION}-win-x64/node.exe', b'node without license')
    monkeypatch.setitem(runtime.ARCHIVE_SHA256, target, runtime.file_sha256(archive))
    bundle = tmp_path / 'bundle'
    bundle.mkdir()
    with pytest.raises(ValueError, match='Invalid Node.js archive member'):
        runtime.stage_node_runtime(
            bundle,
            tmp_path,
            offline=True,
            env={
                'MSINSIGHT_RAG_TARGET_PLATFORM': 'win32',
                'MSINSIGHT_RAG_TARGET_ARCH': 'x64',
            },
        )
    assert not list(bundle.iterdir())


def test_corrupt_cached_archive_is_not_used_or_silently_replaced(tmp_path, monkeypatch):
    archive = tmp_path / runtime.archive_name(('win32', 'x64'))
    archive.write_bytes(b'corrupt')
    monkeypatch.setattr(runtime.urllib.request, 'urlopen', lambda *_args, **_kwargs: pytest.fail('network'))
    with pytest.raises(ValueError, match='SHA-256'):
        runtime.verified_archive(tmp_path, ('win32', 'x64'), False)


@pytest.mark.parametrize('valid', [True, False])
def test_download_is_verified_before_cache_publication(tmp_path, monkeypatch, valid):
    payload = b'archive bytes'
    target = ('win32', 'x64')
    expected = hashlib.sha256(payload if valid else b'other bytes').hexdigest()
    monkeypatch.setitem(runtime.ARCHIVE_SHA256, target, expected)
    requests = []

    def download(url, timeout):
        requests.append((url, timeout))
        return io.BytesIO(payload)

    monkeypatch.setattr(runtime.urllib.request, 'urlopen', download)
    cache = tmp_path / 'cache'
    if valid:
        assert runtime.verified_archive(cache, target, False).read_bytes() == payload
        # Both online and offline builds reuse the published archive without downloading again.
        assert runtime.verified_archive(cache, target, False).read_bytes() == payload
        assert runtime.verified_archive(cache, target, True).read_bytes() == payload
    else:
        with pytest.raises(ValueError, match='SHA-256'):
            runtime.verified_archive(cache, target, False)
        assert not list(cache.iterdir())
    assert requests == [(f'https://nodejs.org/dist/v{runtime.NODE_VERSION}/{runtime.archive_name(target)}', 120)]


@pytest.mark.parametrize('target', [('freebsd', 'x64'), ('win32', 'x86'), ('linux', 'ppc64le')])
def test_unknown_target_fails_explicitly(target):
    with pytest.raises(ValueError, match='Unsupported'):
        runtime.runtime_target({'MSINSIGHT_RAG_TARGET_PLATFORM': target[0], 'MSINSIGHT_RAG_TARGET_ARCH': target[1]})


def test_native_target_aliases_and_libc(monkeypatch):
    monkeypatch.setattr(runtime.platform, 'system', lambda: 'Windows')
    monkeypatch.setattr(runtime.platform, 'machine', lambda: 'AMD64')
    assert runtime.runtime_target({}) == ('win32', 'x64')
    with pytest.raises(ValueError, match='glibc'):
        runtime.runtime_target(
            {
                'MSINSIGHT_RAG_TARGET_PLATFORM': 'linux',
                'MSINSIGHT_RAG_TARGET_ARCH': 'arm64',
                'MSINSIGHT_RAG_TARGET_LIBC': 'musl',
            }
        )


@pytest.mark.parametrize('fails', [False, True])
def test_product_builder_stages_runtime_with_same_target_and_offline_policy(tmp_path, monkeypatch, fails):
    build = load_build_module()
    source = tmp_path / 'modules' / 'insight_web_agent' / 'dist-server'
    source.mkdir(parents=True)
    (source / 'index.mjs').write_text('export {};', encoding='utf-8')
    (source / 'mindstudio-insight-rag.cmd').write_text('development wrapper', encoding='utf-8')
    scripts = source.parent / 'scripts'
    scripts.mkdir()
    (scripts / 'mindstudio-insight-rag.cmd').write_text('packaged wrapper', encoding='utf-8')
    monkeypatch.setattr(build, 'PROJECT_PATH', str(tmp_path))
    monkeypatch.setattr(build, 'exec_command', lambda *_args, **_kwargs: 0)
    target_env = {'MSINSIGHT_RAG_TARGET_PLATFORM': 'win32', 'MSINSIGHT_RAG_TARGET_ARCH': 'x64'}
    monkeypatch.setattr(build, 'rag_subprocess_environment', lambda _context: target_env)
    monkeypatch.setenv('MSINSIGHT_NODE_ARCHIVE_DIR', str(tmp_path / 'cache'))
    calls = []

    def stage(bundle, cache, **options):
        assert (Path(bundle) / 'index.mjs').is_file()
        calls.append((bundle, cache, options))
        if fails:
            raise ValueError('invalid digest')

    monkeypatch.setattr(build, 'stage_node_runtime', stage)
    assert build.build_acp_node_service(SimpleNamespace(offline=True)) == (1 if fails else 0)
    assert calls[0][1] == str(tmp_path / 'cache')
    assert calls[0][2] == {'offline': True, 'env': target_env}
    packaged = tmp_path / 'server' / 'output' / 'build' / 'server' / 'insight_web_agent'
    assert (packaged / 'mindstudio-insight-rag.cmd').read_text(encoding='utf-8') == (
        'development wrapper' if fails else 'packaged wrapper'
    )


@pytest.mark.parametrize('missing', [False, True])
def test_mac_rag_smoke_uses_only_packaged_node(tmp_path, monkeypatch, missing):
    build = load_build_module()
    bundle = tmp_path / '安装目录 with spaces' / 'insight_web_agent'
    (bundle / 'runtime').mkdir(parents=True)
    (bundle / 'rag-required-smoke.mjs').write_text('export {};', encoding='utf-8')
    node = bundle / 'runtime' / 'node'
    if not missing:
        node.write_bytes(b'runtime')
    monkeypatch.setattr(build, 'find_mac_rag_bundle_root', lambda _app: bundle)
    calls = []

    def execute(command, directory, label):
        calls.append(command)
        return 0

    monkeypatch.setattr(build, 'exec_command', execute)
    assert build.verify_mac_packaged_rag(str(tmp_path)) is not missing
    assert calls == ([] if missing else [[str(node), str(bundle / 'rag-required-smoke.mjs')]])
