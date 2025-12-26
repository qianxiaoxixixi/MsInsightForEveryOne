# -*- coding: utf-8 -*-
import os
import shutil
import stat
import subprocess
import time
import traceback
from datetime import datetime

import schedule
import logging
import psutil

from func_timeout import func_set_timeout, exceptions as func_timeout_exception

from email_sender import EmailSender

# 冒烟执行限时2小时，秒为单位
GUI_TEST_TIME_OUT = 2 * 3600
# 编译后台服务10分钟
BUILD_SERVER_TIME_OUT = 600
# 构建前端环境，20分钟
BUILD_FRONTED_TIME_OUT = 1200
# 代码仓
REPO_URL = ''
# # 分支
BRANCH = 'master'


def _get_current_time():
    return datetime.now().strftime('%Y-%m-%d %H:%M:%S')


def _create_logger(log_file_path, logger_name, print_to_screen=False):
    log_file_dir = os.path.dirname(log_file_path)
    os.makedirs(log_file_dir, exist_ok=True)

    logger = logging.getLogger(logger_name)
    for handler in logger.handlers:
        handler.close()
        logger.removeHandler(handler)

    logger.setLevel('INFO')
    formatter = logging.Formatter('%(asctime)s [%(levelname)s] %(message)s', datefmt='%Y-%m-%d %H:%M:%S')

    handler = logging.FileHandler(log_file_path, mode='w')
    handler.setFormatter(formatter)
    logger.addHandler(handler)

    if print_to_screen:
        handler = logging.StreamHandler()
        handler.setFormatter(formatter)
        logger.addHandler(handler)

    return logger


def _remove_readonly(func, path, _):
    """处理只读文件或锁定文件"""
    os.chmod(path, stat.S_IWRITE)
    try:
        func(path)
    except Exception as e:
        print(f"Force remove failed on {path}: {e}")


def _clone_repo(working_dir, repo_name, repo_url, branch):
    working_dir = os.path.expanduser(working_dir)
    if not os.path.exists(working_dir):
        os.makedirs(working_dir)
    repo_path = os.path.join(working_dir, repo_name)
    if os.path.exists(repo_path):
        os.chmod(repo_path, stat.S_IWRITE)
        kill_process_using_path(repo_path)
        time.sleep(3)
        shutil.rmtree(repo_path, onerror=_remove_readonly)

    commands = ['git', 'clone', repo_url, '-b', branch]
    completed_process = subprocess.run(commands, capture_output=True, cwd=working_dir)
    if completed_process.returncode != 0:
        raise RuntimeError(completed_process.stderr.decode("utf-8"))

    return repo_path


def kill_process_using_path(path):
    for proc in psutil.process_iter(['pid', 'name', 'cwd']):
        try:
            if proc.info['cwd'] and path in proc.info['cwd']:
                print(f"Killing process: {proc.info['name']} (pid={proc.info['pid']})")
                proc.kill()
        except (psutil.NoSuchProcess, psutil.AccessDenied):
            continue


def _clone_latest_ascend_insight(working_dir, logger):
    repo_url = REPO_URL
    branch = BRANCH
    repo_name = 'Ascend-Insight'
    logger.info(
        f'Start cloning Ascend-Insight from {repo_url}, checkout to branch {branch}...')
    try:
        ascend_insight_path = _clone_repo(working_dir, repo_name, repo_url, branch)
        logger.info('Success.')
        return ascend_insight_path
    except Exception as e:
        logger.error(e)
        logger.error(traceback.format_exc())
        exit(1)


def _get_latest_commit_id(code_dir):
    commands = ['git', 'rev-parse', 'HEAD']
    completed_process = subprocess.run(commands, capture_output=True, cwd=code_dir)
    if completed_process.returncode != 0:
        raise RuntimeError(f'{completed_process.stderr.decode("utf-8")}')
    commit_id = completed_process.stdout.decode('utf-8').strip()

    return commit_id


def _record_ascend_insight_info(path, logger):
    try:
        commit_id = _get_latest_commit_id(path)
        logger.info(f'Latest commit id: {commit_id}')
    except Exception as e:
        logger.error(f'Get latest commit id failed: {str(e)}')


def _build_server_to_path(code_path, logger):
    logger.info('Start building server')
    build_path = os.path.join(code_path, 'server', 'build')
    if not os.path.exists(build_path):
        raise RuntimeError('Test failed, server build path is not exist')

    command = 'python download_third_party.py && python preprocess_third_party.py && python build.py build'
    process = subprocess.Popen(command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, cwd=build_path)

    def build_backend():
        _, err = process.communicate(timeout=BUILD_SERVER_TIME_OUT)
        if err:
            logger.error(err.decode('utf-8', 'ignore').strip())
            raise RuntimeError(f'Execute command "{command}" failed, return code = {process.returncode}!')

    try:
        build_backend()
        built_server_path = os.path.join(code_path, *['server', 'output', 'win_mingw64', 'bin'])
        if not os.path.exists(os.path.join(built_server_path, 'profiler_server.exe')):
            raise RuntimeError('Test failed, profiler_server.exe is not exist in output path!')
        if not os.path.exists(os.path.join(built_server_path, 'cluster_analysis.exe')):
            raise RuntimeError('Test failed, cluster_analysis.exe is not exist in output path!')
        logger.info('Build server success')
    except subprocess.TimeoutExpired:
        _kill_process(process.pid)
        raise RuntimeError(f'Execute command "{command}" timeout!')


def _build_fronted_env(code_path, logger):
    logger.info('Start building fronted')
    module_path = os.path.join(code_path, 'modules')
    if not os.path.exists(module_path):
        raise RuntimeError('Test failed, modules path is not exist')

    build_script = os.path.join(module_path, 'build', 'build.py')
    command = ['python', build_script]

    try:
        result = subprocess.run(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE, cwd=module_path, check=True)
        logger.info(result.stdout.decode('utf-8').strip())
    except subprocess.CalledProcessError as e:
        logger.error(e.stderr.decode('utf-8', 'ignore').strip())
        raise RuntimeError(f'Execute script "{build_script}" failed, return code = {e.returncode}!')


def _kill_process(pid):
    current_process = psutil.Process(pid)
    for child in current_process.children(recursive=False):
        _kill_process(child.pid)
    try:
        current_process.kill()
    except psutil.NoSuchProcess:
        pass


def _test_gui(working_dir, report_dir, test_results, logger):
    logger.info('Start testing GUI')
    command = 'npm install --force&&npm run test'
    exe_path = os.path.join(working_dir, 'Ascend-Insight', 'e2e')
    if not os.path.exists(exe_path):
        raise RuntimeError('Test failed, dir "e2e" is not exist in the project!')
    process = subprocess.Popen(command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, cwd=exe_path)

    @func_set_timeout(GUI_TEST_TIME_OUT)
    def do_test():
        while process.poll() is None:
            if process.stdout.readable():
                line = process.stdout.readline().decode('utf-8', 'ignore').rstrip()
                if len(line) > 0:
                    logger.info(line)
                if line.startswith('Starting all tests:'):
                    test_results['total'] = int(line.split(' ')[-1])
                if line == 'Finished all tests.':
                    result_dir = os.path.join(exe_path, 'playwright-report')
                    if not os.path.exists(result_dir):
                        raise RuntimeError('Error occurred, playwright-report is not exist!')
                    logger.info('Finished')
                    shutil.copytree(result_dir, os.path.join(report_dir, 'reports'))
                    raise func_timeout_exception.FunctionTimedOut
                if line.startswith('Finished the test:  '):
                    [module, case, status] = line.split('  ')[1:]
                    if status.lower() == 'passed':
                        test_results['passed'] += 1
                        test_results['result'].setdefault(module, {}).setdefault(case, 'pass')
                    else:
                        test_results['result'].setdefault(module, {}).setdefault(case, 'fail')

        if process.returncode != 0:
            raise RuntimeError(f'Execute command "{command}" failed, return code = {process.returncode}!')

    try:
        do_test()
    except func_timeout_exception.FunctionTimedOut:
        _kill_process(process.pid)
        logger.info('Test finished')


def main():
    working_dir = os.path.split(os.path.realpath(__file__))[0]
    start_time = _get_current_time()
    report_dir = os.path.join(working_dir, 'reports')
    if os.path.exists(report_dir):
        shutil.rmtree(report_dir, onerror=_remove_readonly)

    summary_log_path = os.path.join(report_dir, 'smoke_test_summary.log')
    summary_logger = _create_logger(summary_log_path, 'Summary', print_to_screen=True)

    ascend_insight_path = _clone_latest_ascend_insight(working_dir, summary_logger)
    _record_ascend_insight_info(ascend_insight_path, summary_logger)
    test_results = {
        'passed': 0,
        'total': 0,
        'result': {}
    }
    try:
        _build_server_to_path(ascend_insight_path, summary_logger)
        _build_fronted_env(ascend_insight_path, summary_logger)
        _test_gui(working_dir, report_dir, test_results, summary_logger)
    except Exception as e:
        summary_logger.error(str(e))

    summary_logger.info('Send test report email.')
    logging.shutdown()
    if test_results['result']:
        attachment_path = shutil.make_archive(os.path.join(working_dir, 'reports'), 'zip', report_dir)
    else:
        attachment_path = summary_log_path
    sender = ''  # 发件邮箱
    username = ''  # 电脑的账号和密码
    password = ''  # 密码
    receivers = []
    end_time = _get_current_time()
    email = EmailSender.generate_email_content(test_results, start_time, end_time, BRANCH)
    EmailSender(sender, username, password).send(email.get('subject'), email.get('html_content'),
                                                 attachment_path, receivers, [])


if __name__ == '__main__':
    # TODO: for debug
    main()
