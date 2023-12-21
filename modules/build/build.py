#!/usr/bin/env python
# -*- coding: UTF-8 -*-
#
# Copyright 2023 Huawei Technologies Co., Ltd
# ============================================================================
#
# build modules

import logging
import multiprocessing
import os
import platform
import shutil
import subprocess

BUILD_DIR = os.path.dirname(os.path.abspath(__file__))
MODULES_DIR = os.path.dirname(BUILD_DIR)
PLUGIN_DIR = os.path.join(os.path.dirname(MODULES_DIR), 'framework', 'plugins')

MODULES_MAP = {
    'cluster': 'Cluster',
    'memory': 'Memory',
    'operator': 'Operator',
    'timeline': 'Timeline'
}


def build_module(module):
    logging.info('start to build %s', module)
    module_dir = os.path.join(MODULES_DIR, module)
    plugin_dir = os.path.join(PLUGIN_DIR, MODULES_MAP.get(module))
    if os.path.exists(plugin_dir):
        shutil.rmtree(plugin_dir)

    npm_cmd = 'npm.cmd' if platform.system() == 'Windows' else 'npm'
    result = subprocess.run([npm_cmd, 'install', '--force'], cwd=module_dir)
    if result.returncode != 0:
        logging.error(result.stderr)
        logging.error('failed to build %s due to download dependency', module)
        return
    result = subprocess.run([npm_cmd, 'run', 'build'], cwd=module_dir)
    if result.returncode != 0:
        logging.error(result.stderr)
        logging.error('failed to build %s due to build project', module)
        return
    shutil.copytree(os.path.join(module_dir, 'build'), plugin_dir)
    logging.info('finish to build %s', module)


def parallel_build():
    modules = list(MODULES_MAP.keys())

    pool = multiprocessing.Pool(processes=min(multiprocessing.cpu_count(), 4))
    results = pool.map(build_module, modules)

    pool.close()
    pool.join()

    logging.info("Results: ", results)


def main():
    os.putenv('npm_config_build_from_source', 'true')
    os.putenv('npm_config_audit', 'false')
    os.putenv('npm_config_strict_ssl', 'false')
    os.putenv('npm_config_disturl', 'http://mirrors.tools.huawei.com/nodejs')
    os.putenv('npm_config_registry', 'https://cmc.centralrepo.rnd.huawei.com/artifactory/api/npm/npm-central-repo/')

    parallel_build()


if __name__ == '__main__':
    if platform.system() != 'Windows':
        multiprocessing.set_start_method('fork')
    main()
