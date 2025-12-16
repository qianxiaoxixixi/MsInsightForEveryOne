compare_accuracy() {
    file1=$1
    file2=$2
    while IFS= read -r line1 && IFS= read -r line2 <&3; do
    if [ "$line1" != "$line2" ]; then
        if (echo "$line1" | grep -q -E '"md5"|"group_id"') && (echo "$line2" | grep -q -E '"md5"|"group_id"'); then
            continue
        elif (echo "$line1" | grep -q -E '"md5"|"group_id"') || (echo "$line2" | grep -q -E '"md5"|"group_id"'); then
            echo $line1
            return 1
        fi

        if ! (echo "$line1" | grep -q -E '"Max":| "Min":| "Mean":| "Norm":| "value":| "Max_except_inf_nan":| "Min_except_inf_nan":') || \
            ! (echo "$line2" | grep -q -E '"Max":| "Min":| "Mean":| "Norm":| "value":| "Max_except_inf_nan":| "Min_except_inf_nan":'); then
            echo HAHAHAHHAHA
            echo $line1
            return 1
        fi

        str1=$(echo "$line1" | awk -F'[:,]' '{print $2}')
        str2=$(echo "$line2" | awk -F'[:,]' '{print $2}')

        path=$(dirname $(readlink -f "$0"))
        result=$(python3 "$path/utils/compare_accuracy.py" $str1 $str2)
        if [[ "$result" -eq 1 ]]; then
            echo $result
            return $result
        else
            continue
        fi
    fi
    done <"$file1" 3<"$file2"
    return 0
}



compare_accuracy dump.json dump_baseline.json
