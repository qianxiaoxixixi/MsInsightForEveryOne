# MindStudio Insight采集Linux Kernel数据联合分析Host Bound问题
## 问题背景
在大模型中，CPU主要负责任务的下发，NPU负责计算任务的执行。无论训练还是推理领域，Host Bound都是现网高发问题。分析Host Bound问题通常需要采集Linux Kernel ftrace数据，分析CPU上的进程调度情况。

目前缺乏一种工具，能够统合profiling数据和Linux Kernel ftrace数据，做到联合分析。MindStudio Insight在本仓库中，提出一些工具脚本，帮助开发者实现两种数据的联合分析，提高Host Bound问题定位的效率。

## 特性

+ 支持**命令行**和**API接口**两种方式的ftrace数据采集
+ 支持将ftrace格式数据转换为Chrome Trace Json格式，并导入MindStudio Insight进行可视化展示

## Host Bound问题定位思路

1. 绑核、流水优化、内存分配库替换三板斧，参考：[调度优化-Ascend Extension for PyTorch-昇腾社区](https://www.hiascend.com/document/detail/zh/Pytorch/720/ptmoddevg/trainingmigrguide/performance_tuning_0059.html)
2. 同时采集ftrace和profiling数据
3. 将ftrace数据转换为MindStudio Insight可识别的数据格式
4. 同时导入ftrace数据与profiling数据，分析进程调度情况

## 模型Profiling采集

参考昇腾社区有关profiling采集相关的文档：[简介-CANN商用版8.3.RC1-昇腾社区](https://www.hiascend.com/document/detail/zh/canncommercial/83RC1/devaids/Profiling/atlasprofiling_16_0001.html)

## Linux Kernel ftrace数据采集

### 1. Linux Kernel ftrace数据介绍

Linux内核内置了多种跟踪（trace）工具，其中ftrace作为从2.6.27版本开始引入主流内核的跟踪框架，可用于监控和调试内核中发生的各类事件，帮助开发人员深入分析系统运行时的内部行为。ftrace支持多种跟踪器，例如函数调用跟踪、上下文切换跟踪、中断延迟分析等，能够有效辅助定位内核态性能问题与调度异常。在本仓库中，仅开启了与CPU进程调度相关的事件（sched）。具体输出如下：

```
# tracer: nop
#
# entries-in-buffer/entries-written: 1839083/1839083   #P:180
#
#                                _-----=> irqs-off
#                               / _----=> need-resched
#                              | / _---=> hardirq/softirq
#                              || / _--=> preempt-depth
#                              ||| /     delay
#           TASK-PID     CPU#  ||||   TIMESTAMP  FUNCTION
#              | |         |   ||||      |         |
          <idle>-0       [100] dN.. 4910289.782507: sched_stat_wait: comm=libuv-worker pid=1467836 delay=0 [ns]
          <idle>-0       [100] d... 4910289.782508: sched_switch: prev_comm=swapper/100 prev_pid=0 prev_prio=120 prev_state=R ==> next_comm=libuv-worker next_pid=1467836 next_prio=120
           <...>-1467836 [100] d... 4910289.782514: sched_waking: comm=node pid=1467818 prio=120 target_cpu=102
           <...>-1467836 [100] d... 4910289.782515: sched_stat_sleep: comm=node pid=1467818 delay=6155 [ns]
           <...>-1467836 [100] d... 4910289.782517: sched_wake_idle_without_ipi: cpu=102
           <...>-1467836 [100] d... 4910289.782517: sched_wakeup: comm=node pid=1467818 prio=120 target_cpu=102
           <...>-1467836 [100] d... 4910289.782519: sched_stat_runtime: comm=libuv-worker pid=1467836 runtime=12816 [ns] vruntime=114248331523986 [ns]
           <...>-1467836 [100] d... 4910289.782519: sched_switch: prev_comm=libuv-worker prev_pid=1467836 prev_prio=120 prev_state=S ==> next_comm=swapper/100 next_pid=0 next_prio=120
          <idle>-0       [100] dN.. 4910289.782596: sched_stat_wait: comm=libuv-worker pid=1467836 delay=0 [ns]
          <idle>-0       [100] d... 4910289.782597: sched_switch: prev_comm=swapper/100 prev_pid=0 prev_prio=120 prev_state=R ==> next_comm=libuv-worker next_pid=1467836 next_prio=120
           <...>-1467836 [100] d... 4910289.782603: sched_waking: comm=node pid=1467818 prio=120 target_cpu=102
```



各种事件代表了不同的含义，这里我们主要关注以下几类事件：

+ sched_switch: 记录了CPU上的进程切换
+ sched_wakeup: 进程被唤醒
+ sched_wakeup_new: 新创建的进程首次被唤醒
+ shced_process_fork/shced_process_exec/sched_process_exit: 进程创建销毁

trace-cmd是一个命令行工具，它封装了trace采集的过程，提供了更加简易的命令接口。

### 2.Linux Kernel数据采集操作

#### 前置准备

+ 安装trace-cmd命令

  Ubuntu安装命令：`sudo apt-get install trace-cmd`

  CentOs安装命令：`sudo yum install trace-cmd`

+ 获取仓库中提供的采集脚本`trace_record.py`，推荐profiling和ftrace数据同步采集。

#### 采集方式一：通过命令行方式采集

这种方式不需要修改现有代码，将trace_record脚本作为整体使用，能够快速上手，但是缺点在于灵活性较低。

trace_record.py作为整体脚本使用时提供以下参数：

```
usage: trace_record.py [-h] [--cpu CPU] [--output OUTPUT] --record_time RECORD_TIME

options:
  -h, --help            show this help message and exit
  --cpu CPU
  --output OUTPUT
  --record_time RECORD_TIME
                        record time, if pass <=0, will start long term record that user should attension the disk space

```

+ cpu: cpu_mask列表，多个cpu使用逗号分割，例如准备采集CPU0，1，4的数据，则传入--cpu=0,1,4
+ output:输出文件名
+ record_time:采集事件，单位秒.如果传递小于等于0的数字，则会持续采集，知道ctrl-c终止进程

Example:

1. 在终端中启动采集脚本`python trace_record.py --record_time=60`
2. 在另一终端中启动训练脚本`python train.py`

#### 侵入式采集

trace_record中提供两个接口，分别控制ftrace采集的启停，可以更灵活的控制采集逻辑

**开始采集接口**

```
def ftrace_record_start(cpu_list:[])
```

+ cpu_list: cpu采集列表，默认采集全部数据, 示例：[0 , 1, 4]

**停止采集保存数据接口**

```
def ftrace_record_stop(output)
```

+ output： 输出文件

Example:

```python
import ftrace_record
def train()
	ftrace_record_start(cpu=[0,1,4])
    profiling_start()
    // 训练逻辑
    profiling_stop()
    ftrace_record_stop(output="ftrace.txt")
```

### 3.数据采集后处理

ftrace_convert脚本提供了ftrace数据格式转换功能，使用方法如下：

```
usage: trace_convert.py [-h] [--input INPUT] [--output OUTPUT] [--cpu_list CPU_LIST] [--profiling_data PROFILING_DATA]

options:
  -h, --help            show this help message and exit
  --input INPUT
  --output OUTPUT
  --cpu_list CPU_LIST
  --profiling_data PROFILING_DATA
                        use profiling data to adjust start time
```

+ input: 输入文件，recrod接口采集的ftrace.txt
+ output： 输出文件，默认为trace.json
+ cpu_list: 过滤的cpu列表，建议在采集时过滤而非解析时
+ profiling_data: 同步采集的profiling数据，用于时间轴对齐

## 联合分析

1. 在MindStudio Insight中导入profiling数据
2. 在profiling数据中点击添加卡数据，将trace.json加入到工程中