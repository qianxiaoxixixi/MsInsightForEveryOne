#!/usr/bin/env python
# -*- coding: UTF-8 -*-

# Copyright 2024 Huawei Technologies Co., Ltd
# ============================================================================

import os
import sys
import json
import subprocess
import socket
import uuid
import logging
from jupyter_server.base.handlers import APIHandler
from jupyter_server.utils import url_path_join
import tornado
from tornado.web import StaticFileHandler


# 配置日志
logging.basicConfig(
    level=logging.INFO,
    format='%(asctime)s - %(levelname)s - %(message)s',
    handlers=[logging.StreamHandler()]
)
# init profiler_server process
profiler_process = None
# default profiler_server port
available_port = 9000
# generate random token
gen_token = str(uuid.uuid4())


def find_available_port(start_port=9000, host='127.0.0.1', max_tries=100):
    global available_port
    available_port = start_port
    tries = 0
    while tries < max_tries:
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            try:
                s.bind((host, available_port))
                return available_port
            except socket.error:
                available_port += 1
                tries += 1
                continue


def start_profiler_server():
    user_home_dir = os.path.expanduser('~')
    mindstudio_insight_dir = os.path.join(user_home_dir, '.mindstudio_insight')

    if not os.path.exists(mindstudio_insight_dir):
        os.makedirs(mindstudio_insight_dir, 0o750)

    global profiler_process
    profiler_server_path = os.path.join(os.path.dirname(__file__), 'resources', 'server', 'profiler_server')

    global available_port
    available_port = find_available_port()
    # 配置参数
    command = [
        profiler_server_path, '--wsPort', str(available_port),
        '--wsHost', '0.0.0.0', '--logPath', mindstudio_insight_dir
    ]

    if sys.platform == 'win32':
        profiler_server_path = profiler_server_path + '.exe'
        # 设置执行权限
        os.chmod(profiler_server_path, 0o755)
        command[0] = profiler_server_path
        # start profiler server and set port
        profiler_process = subprocess.Popen(command)
    else:
        # 设置执行权限
        os.chmod(profiler_server_path, 0o755)
        server_dir = os.path.join(os.path.dirname(__file__), 'resources', 'server')
        env = os.environ.copy()
        env["LD_LIBRARY_PATH"] = f".:{env.get('LD_LIBRARY_PATH', '')}"
        command[0] = './profiler_server'
        profiler_process = subprocess.Popen(command, cwd=server_dir, env=env)


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


class TokenHandler(APIHandler):
    @tornado.web.authenticated
    def get(self):
        # generate random token
        global gen_token
        # logging info for token
        logging.info(f"To access the mindstudio insight server, you should take this token: {gen_token}")
        self.finish(json.dumps({
            "token": gen_token
        }))


class PortHandler(APIHandler):
    @tornado.web.authenticated
    def get(self):
        # find available port
        global available_port
        self.finish(json.dumps({
            "port": available_port
        }))


class RouteHandler(APIHandler):
    # The following decorator should be present on all verb methods (head, get, post,
    # patch, put, delete, options) to ensure only authorized user can request the
    # Jupyter server
    @tornado.web.authenticated
    def get(self):
        self.finish(json.dumps({
            "data": "This is mindstudio_insight_jupyterlab get_example endpoint!"
        }))


class TokenAwareStaticFileHandler(StaticFileHandler):
    def prepare(self):
        # 获取请求url
        request_uri = self.request.uri
        # 获取请求中的token
        token = self.get_argument("token", None)

        global gen_token
        if '/resources/frontend/index.html' in request_uri:
            if not token or token != gen_token:
                self.set_status(403)
                self.finish(json.dumps({
                    "error": "Invalid or missing token"
                }))
                return

        super().prepare()


def setup_handlers(web_app):
    web_app.settings["shutdown_hook"] = shutdown_hook
    host_pattern = ".{1,255}$"
    base_url = web_app.settings["base_url"]

    token_route_pattern = url_path_join(base_url, "/mindstudio_insight_jupyterlab/get_token")
    token_handlers = [(token_route_pattern, TokenHandler)]

    port_route_pattern = url_path_join(base_url, "/mindstudio_insight_jupyterlab/get_available_port")
    port_handlers = [(port_route_pattern, PortHandler)]

    static_frontend_path = os.path.join(os.path.dirname(os.path.realpath(__file__)), 'resources', 'frontend')
    static_route_pattern = url_path_join(base_url, "/resources/frontend/(.*)")
    static_handlers = [
        (static_route_pattern, TokenAwareStaticFileHandler, {'path': static_frontend_path})
    ]

    api_route_pattern = url_path_join(base_url, "/mindstudio_insight_jupyterlab/get_example")
    api_handlers = [(api_route_pattern, RouteHandler)]

    web_app.add_handlers(host_pattern, token_handlers + port_handlers + static_handlers + api_handlers)

    start_profiler_server()
