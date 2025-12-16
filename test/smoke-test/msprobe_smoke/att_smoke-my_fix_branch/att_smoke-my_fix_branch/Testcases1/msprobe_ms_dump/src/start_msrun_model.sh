rm -rf msrun_log
mkdir msrun_log
echo "start training"
cur_dir=$(dirname $(readlink -f $0))
echo $cur_dir
echo "当前目录: $cur_dir"
cd $cur_dir
msrun --worker_num=4 --local_worker_num=4 --master_port=8418 --log_dir=msrun_log --join=True --cluster_time_out=300 ${cur_dir}/train_model.py ${cur_dir}/config.json
