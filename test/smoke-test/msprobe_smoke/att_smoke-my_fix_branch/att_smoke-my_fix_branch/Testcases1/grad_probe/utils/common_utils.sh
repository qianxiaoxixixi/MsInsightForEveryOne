#!/bin/bash

# Requires one parameters: error information
handle_error() {
    echo "[ERROR] $1"
    end_time=$(date "+%s")
    duration_time=$(( ${end_time} - ${start_time} ))
    echo "${file_name} fail ${duration_time} ${pytorch_branchs}"
    exit 1
}
