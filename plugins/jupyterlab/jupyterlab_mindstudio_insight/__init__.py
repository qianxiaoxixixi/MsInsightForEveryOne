#!/usr/bin/env python
# -*- coding: UTF-8 -*-

# Copyright 2024 Huawei Technologies Co., Ltd
# ============================================================================

from .handlers import setup_handlers


def _jupyter_labextension_paths():
    return [{
        "src": "labextension",
        "dest": "jupyterlab_mindstudio_insight"
    }]


def _jupyter_server_extension_points():
    return [{
        "module": "jupyterlab_mindstudio_insight"
    }]


def _load_jupyter_server_extension(server_app):
    setup_handlers(server_app.web_app)
    name = "jupyterlab_mindstudio_insight"
    server_app.log.info(f"Registered {name} server extension")

# For backward compatibility with notebook server - useful for Binder/JupyterHub
load_jupyter_server_extension = _load_jupyter_server_extension
