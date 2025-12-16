#!/bin/bash

compare_md5() {
    input_file=$1
    baseline_file=$2
    output_dir=$3

    input_md5=$(md5sum $input_file | awk '{print $1}')
    baseline_md5=$(md5sum $baseline_file | awk '{print $1}')

    if [ $input_md5 != $baseline_md5 ]; then
        cp $input_file $output_dir/$(basename $baseline_file)
        return 1
    fi
    return 0
}