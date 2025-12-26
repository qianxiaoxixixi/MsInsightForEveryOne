#!/bin/bash

source ./scriptB.sh
# 定义全局变量
GLOBAL_VAR1="Hello"
GLOBAL_VAR2="World"

# 引入脚本 B
#source ./scriptB.sh

# 调用函数
function_from_B1
function_from_B2

