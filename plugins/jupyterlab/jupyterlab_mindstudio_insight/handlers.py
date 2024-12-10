#!/usr/bin/env python
# -*- coding: UTF-8 -*-

# Copyright 2024 Huawei Technologies Co., Ltd
# ============================================================================

import os
import sys
import json
import subprocess
from jupyter_server.base.handlers import APIHandler
from jupyter_server.utils import url_path_join
import tornado
from tornado.web import StaticFileHandler


# init profiler_server process
profiler_process = None


def start_profiler_server():
    user_home_dir = os.path.expanduser('~')
    mindstudio_insight_dir = os.path.join(user_home_dir, '.mindstudio_insight')

    if not os.path.exists(mindstudio_insight_dir):
        os.makedirs(mindstudio_insight_dir, 0o750)

    global profiler_process
    profiler_server_path = os.path.join(os.path.dirname(__file__), 'resources', 'server', 'profiler_server')

    # 配置参数
    command = [profiler_server_path, '--wsPort', '9000', '--logPath', mindstudio_insight_dir]

    if sys.platform == 'win32':
        profiler_server_path = profiler_server_path + '.exe'
        command[0] = profiler_server_path

    # start profiler server and set port
    profiler_process = subprocess.Popen(command)


def stop_profiler_server():
    global profiler_process
    if profiler_process:
        try:
            profiler_process.terminate()
            profiler_process.wait(timeout=3)
        except subprocess.TimeoutExpired:
            profiler_process.kill()  # 强制终止进程
        profiler_process = None


def shutdown_hook(web_app):
    stop_profiler_server()


class RouteHandler(APIHandler):
    # The following decorator should be present on all verb methods (head, get, post,
    # patch, put, delete, options) to ensure only authorized user can request the
    # Jupyter server
    @tornado.web.authenticated
    def get(self):
        self.finish(json.dumps({
            "data": "This is jupyterlab_mindstudio_insight get_example endpoint!"
        }))


def setup_handlers(web_app):
    web_app.settings["shutdown_hook"] = shutdown_hook
    host_pattern = ".{1,255}$"
    base_url = web_app.settings["base_url"]

    static_frontend_path = os.path.join(os.path.dirname(os.path.realpath(__file__)), 'resources', 'frontend')
    static_route_pattern = url_path_join(base_url, "/resources/frontend/(.*)")
    static_handlers = [
        (static_route_pattern, StaticFileHandler, {'path': static_frontend_path})
    ]

    api_route_pattern = url_path_join(base_url, "/jupyterlab-mindstudio-insight/get_example")
    api_handlers = [(api_route_pattern, RouteHandler)]
    web_app.add_handlers(host_pattern, static_handlers + api_handlers)

    start_profiler_server()
