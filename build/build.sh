#!/bin/bash
#Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
# ================================================================================
set -e

script=$(readlink -f "$0")
route=$(dirname "$script")
root="${route}"/..

modules=(Cluster Memory Timeline)
vscode_version=6.0.3
idea_version=6.0.RC3
build_type=$1

function init() {
  mkdir -p "${root}"/out && rm -rf "${root}"/out/*
  mkdir -p "${root}"/plugins/vscode/profiler && rm -rf "${root}"/plugins/vscode/profiler/*
  mkdir -p "${root}"/serverBuild/server && rm -rf "${root}"/serverBuild/server
  os=$(uname -s)
  # 优先读取交叉编译设置
  if [ "${build_type}" == 'cross_compile' ]; then
    pythonExe=python3
    os_name=win
  elif [ "${os:0:5}" == 'MINGW' ]; then
    pythonExe=python
    os_name=win
  elif [ "${os:0:5}" == 'Linux' ]; then
    pythonExe=python3
    os_name=linux-"$(arch)"
  else
    echo "${os:0:5}"':os system is not support' && exit 1
  fi
}

function buildFramework() {
  cd "${root}"/framework && rm -rf dist
  npm install && npm run build
  mkdir -p "${root}"/plugins/vscode/profiler && rm -rf "${root}"/plugins/vscode/profiler/*
  cp -fr "${root}"/framework/dist/* "${root}"/plugins/vscode/profiler/
}

function buildModules() {
  mkdir -p "${root}"/framework/plugins && rm -rf "${root}"/framework/plugins/*
  cd "${root}"/modules
  for module in ${modules[*]}; do
    path="${root}"/modules/"${module,,}"
    cd "${path}" && rm -rf build && npm install --force && npm run build
    cp -rf "${path}"/build "${root}"/framework/plugins/"${module}"
  done
  cp -rf "${root}"/framework/plugins "${root}"/plugins/vscode/profiler/
}

function buildServer() {
  cd "${root}"/server/build
  ${pythonExe} download_third_party.py
  ${pythonExe} build.py clean
  if [ "${build_type}" == 'cross_compile' ]; then
    ${pythonExe} build.py build --release --cross_compile
  else
    ${pythonExe} build.py build --release
  fi
  cp -fr "${root}"/server/output/"${os_name}"*/bin "${root}"/serverBuild/server
}

function buildVscode() {
  rm -fr cd "${root}"/plugins/vscode/dist
  cd "${root}"/plugins/vscode && npm install --force && npm run vsce:package
  cp "${root}"/plugins/vscode/ascend-insight-extension-* "${root}"/out/ascend-insight-extension_"${vscode_version}"_"${os_name}".vsix
  # 打包中间件，方便构建轻量化exe
  cd "${root}"/plugins/vscode/dist
  tar -czvf "${root}"/out/ascend-insight_"${vscode_version}"_"${os_name}".tar.gz profiler
}

function buildIntellij() {
  cd "${root}"/plugins/intellij
  if [ 0"$GRADLE_URL" = "0" ]; then
    gradle wrapper
  else
    gradle wrapper --gradle-distribution-url "$GRADLE_URL"
  fi
  chmod a+x gradlew
  ./gradlew clean || {
    echo "gradlew clean ascend failed" && exit 2
  }
  ./gradlew build || {
    echo "build ascend failed" && exit 4
  }
  cp "${root}"/plugins/intellij/build/distributions/ascend-insight-* "${root}"/out/ascend-insight-plugin_"${idea_version}"_"${os_name}".zip
}

function main() {
  export npm_config_build_from_source=true
  export npm_config_audit=false
  export npm_config_strict_ssl=false
  export npm_config_disturl=http://mirrors.tools.huawei.com/nodejs
  export npm_config_registry=https://cmc.centralrepo.rnd.huawei.com/artifactory/api/npm/npm-central-repo/
  init
  buildFramework
  buildModules
  buildServer
  buildVscode
  buildIntellij
}

main