#!/bin/bash
#Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
# ================================================================================
set -e

script=$(readlink -f "$0")
route=$(dirname "$script")
root="${route}"/..

modules=(cluster Memory Timeline)

function buildFramework() {
  cd "${root}"/framework
#  npm install && npm run build
  mkdir -p "${root}"/plugins/vscode/profiler && rm -rf "${root}"/plugins/vscode/profiler/*
  cp -fr "${root}"/framework/dist/* "${root}"/plugins/vscode/profiler/
}

function buildModules() {
  mkdir -p "${root}"/modules/plugins && rm -rf "${root}"/modules/plugins/*
  cd "${root}"/modules
  for module in ${modules[*]}; do
    path="${root}"/modules/"${module,,}"
#    cd "${path}" && npm install --force && npm run build
    cp -rf "${path}"/build "${root}"/modules/plugins/"${module}"
  done
  cp -rf "${root}"/modules/plugins "${root}"/plugins/vscode/profiler/
}


function main() {
  export npm_config_build_from_source=true
  export npm_config_audit=false
  export npm_config_strict_ssl=false
  export npm_config_disturl=http://mirrors.tools.huawei.com/nodejs
  export npm_config_registry=https://cmc.centralrepo.rnd.huawei.com/artifactory/api/npm/npm-central-repo/
  buildFramework
  buildModules
}

main