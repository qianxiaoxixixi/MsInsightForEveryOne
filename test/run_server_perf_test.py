#!/usr/bin/env python
# -*- coding: UTF-8 -*-
#
# Copyright 2024 Huawei Technologies Co., Ltd
# ============================================================================
# run server performance test script
import logging
import os
import platform
import shutil
import stat
import subprocess
import sys

SCRIPT_PATH = os.path.realpath(__file__)
PROJECT_PATH = os.path.dirname(SCRIPT_PATH)


class Const:
    WINDOWS_OS = 'Windows'
    MAC_OS = 'Darwin'
    LINUX_OS = 'Linux'
    ASCEND_INSIGHT_DIR = 'Ascend-Insight'
    TEST_DIR = 'test'
    SERVER_DIR = 'server'
    TEST_SCRIPT = 'server_perf_test.py'
    CODE_PATH = os.path.join(PROJECT_PATH, ASCEND_INSIGHT_DIR)
    TEST_PATH = os.path.join(CODE_PATH, TEST_DIR)
    GIT = 'git.exe' if platform.system() == WINDOWS_OS else 'git'
    PYTHON = 'python' if platform.system() == WINDOWS_OS else 'python3'


def exec_command(cmd, workspace):
    logging.info('Execute %s in the %s', cmd, workspace)
    proc = subprocess.Popen(cmd, cwd=workspace, stdout=subprocess.PIPE)
    for line in iter(proc.stdout.readline, b''):
        logging.info(line.decode('utf-8').strip())
    proc.communicate(timeout=6000)
    return proc.returncode


def init():
    clean()

    # 测试工程，下载代码
    url = 'ssh://git@codehub-dg-y.huawei.com:2222/mindstudio/MindStudio-IDE/Ascend-Insight.git'
    result = exec_command([Const.GIT, 'clone', '--depth=1', url], workspace='./')
    if result != 0:
        logging.error('Failed to download code.')
    return result


def chmod_dirs(path):
    if not os.path.exists(path):
        return
    for root, dirs, files in os.walk(path):
        for tmp in dirs:
            os.chmod(os.path.join(root, tmp), stat.S_IWRITE)
        for tmp in files:
            os.chmod(os.path.join(root, tmp), stat.S_IWRITE)


def clean():
    if not os.path.exists(Const.CODE_PATH):
        return

    top_git_pack_path = os.path.join(Const.CODE_PATH, '.git', 'objects')
    chmod_dirs(top_git_pack_path)

    third_party_path = os.path.join(Const.CODE_PATH, Const.SERVER_DIR, 'third_party')
    if not os.path.exists(third_party_path):
        shutil.rmtree(Const.CODE_PATH)
        return

    for directory in os.listdir(third_party_path):
        git_object_path = os.path.join(third_party_path, directory, '.git', 'objects')
        chmod_dirs(git_object_path)

    shutil.rmtree(Const.CODE_PATH)


def main():
    logging.basicConfig(level=logging.INFO)

    result = init()
    if result != 0:
        return -1

    # 拉起运行脚本
    result = exec_command([Const.PYTHON, Const.TEST_SCRIPT], Const.TEST_PATH)
    if result != 0:
        logging.error('Failed to run server performance test.')

    clean()
    return 0


if __name__ == "__main__":
    sys.exit(main())
