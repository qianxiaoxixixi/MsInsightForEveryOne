rm -rf msrun_log
mkdir msrun_log
echo "start training"

msrun --worker_num=4 --local_worker_num=4 --master_port=8118 --log_dir=msrun_log --join=True --cluster_time_out=300 train_model.py
