/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2023. All rights reserved.
 */

#include <iostream>
#include "ThreadPool.h"

void ThreadPool::threadFunc(ThreadPool &threadPool, int index)
{
    std::function<void()> func;
    bool hasTask = false;
    while(threadPool.running) {
        {
            std::unique_lock<std::mutex> lock(threadPool.taskMutex);
            if (threadPool.taskQueue.Empty()) {
                threadPool.taskCv.wait(lock, [&threadPool](){return !threadPool.running || !threadPool.taskQueue.Empty();});
            }
            hasTask = threadPool.taskQueue.Pop(func);
        }
        std::cout << "work " << index << " run. hasTask:" << hasTask << std::endl;
        if (hasTask) {
            threadPool.runningTasks++;
            func();
            threadPool.runningTasks--;
        }
        if (threadPool.waiting && threadPool.runningTasks == 0 && threadPool.taskQueue.Empty()) {
            threadPool.taskDoneCv.notify_all();
        }
    }
    std::cout << "work " << index << " exit" << std::endl;
}

ThreadPool::ThreadPool(int threadCount)
{
    for (int i = 0; i < threadCount; i++) {
        threads.emplace_back(ThreadPool::threadFunc, std::ref(*this), i);
    }
}

ThreadPool::~ThreadPool()
{
    ShutDown();
}

void ThreadPool::Reset()
{
    taskQueue.Clear();
    WaitForAllTasks();
}

void ThreadPool::ShutDown()
{
    if (running) {
        WaitForAllTasks();
        taskQueue.Clear();
        running = false;
        taskCv.notify_all();
        std::cout << "thread notify_all" << std::endl;
        for(auto &thread : threads) {
            if (thread.joinable()) {
                thread.join();
            }
        }
        threads.clear();
    }
}

void ThreadPool::WaitForAllTasks()
{
    std::unique_lock<std::mutex> lock(taskMutex);
    waiting = true;
    taskDoneCv.wait(lock, [this](){return runningTasks == 0 && taskQueue.Empty();});
    waiting = false;
}
