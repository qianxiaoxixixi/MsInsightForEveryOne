src_dir="/home/maoyanlong/att_smoke_new/Testcases1/msprobe_ms_dump/json"
out_file="/home/maoyanlong/att_smoke_new/Testcases1/msprobe_ms_dump/json/level_1.json"

{
    echo '"level_1": ['
    first=true
    for f in "${src_dir}"/mindtorch*.json; do
        fname=$(basename "$f")
        sh_name="${fname%.json}.sh"
        if $first; then
            echo "  \"${sh_name}\""
            first=false
        else
            echo "  ,\"${sh_name}\""
        fi
    done
    echo ']'
} > "$out_file"

echo "✅ 已生成 $out_file"

