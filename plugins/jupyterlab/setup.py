#!/usr/bin/env python
# -*- coding: UTF-8 -*-

# Copyright 2024 Huawei Technologies Co., Ltd
# ============================================================================

from pathlib import Path
import json
from jupyter_packaging import (
    wrap_installers,
    npm_builder,
    get_data_files
)
import setuptools

HERE = Path(__file__).parent.resolve()

# The name of the project
PROJECT_NAME = "jupyterlab_mindstudio_insight"

lab_path = (HERE / PROJECT_NAME / "labextension")
config_path = (HERE / "jupyter-config" / "server-config")

# Representative files that should exist after a successful build
jstargets = [
    str(lab_path / "package.json"),
]

# 指定要复制的文件进行打包
data_files_spec = [
    (f"share/jupyter/labextensions/{PROJECT_NAME}", str(lab_path), "**/*"),
    (f"share/jupyter/labextensions/{PROJECT_NAME}", str(HERE), "install.json"),
    (f"etc/jupyter/jupyter_server_config.d", str(config_path), "jupyterlab_mindstudio_insight.json")
]
data_files = get_data_files(data_files_spec)

# frontend build command
builder = npm_builder(HERE, build_cmd="build:prod", npm=["jlpm"])

# 使用 wrap_installers 代替 create_cmdclass
cmdclass = wrap_installers(
    pre_dist=builder,
    ensured_targets=jstargets
)

long_description = (HERE / "README.md").read_text()

# Get the package info from package.json
pkg_json = json.loads((HERE / "package.json").read_bytes())

package_data_spec = {
    PROJECT_NAME: ["*"],
}

setup_args = dict(
    name=PROJECT_NAME,
    version=pkg_json["version"],
    url=pkg_json["homepage"],
    description=pkg_json["description"],
    author=pkg_json["author"]["name"],
    author_email=pkg_json["author"]["email"],
    license=pkg_json["license"],
    long_description=long_description,
    long_description_content_type="text/markdown",
    package_data=package_data_spec,
    cmdclass=cmdclass,
    data_files=data_files,
    packages=setuptools.find_packages(),
    install_requires=[
        "jupyterlab=3.6.8",
        "tornado<=6.2",
    ],
    zip_safe=False,
    include_package_data=True,
    python_requires=">=3.6",
    platforms="Linux, Mac OS X, Windows",
    keywords=["Jupyter", "JupyterLab", "JupyterLab3", "MindStudio", "MindStudio_Insight"],
    entry_points={
        'console_scripts': [
            'jupyterlab-mindstudio-insight = jupyterlab_mindstudio_insight.application:main',
         ],
    },
    classifiers=[
        'Intended Audience :: Developers',
        'Intended Audience :: Science/Research',
        "License :: OSI Approved :: BSD License",
        "Framework :: Jupyter",
        "Programming Language :: Python :: 3.11",
        "Programming Language :: Python :: 3.10",
        "Programming Language :: Python :: 3.9",
        "Programming Language :: Python :: 3.8",
        "Programming Language :: Python :: 3.7",
        "Programming Language :: Python :: 3.6",
        "Programming Language :: Python :: 3",
        "Programming Language :: Python",
    ],
)

if __name__ == "__main__":
    setuptools.setup(**setup_args)

