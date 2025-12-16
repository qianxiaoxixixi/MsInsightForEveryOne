


replace_json() {
    local dump_path=$1
    local dump_config_path=$2
    
    mkdir -p ${dump_path}
    sed -i "s#\"dump_path\".*#\"dump_path\":\"${dump_path}\",#" ${dump_config_path}
}