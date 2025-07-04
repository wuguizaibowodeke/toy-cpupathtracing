#pragma once

#include "thread/spin_lock.hpp"
#include <functional>
#include <vector>
#include <thread>
#include <queue>

class Task
{
public:
    virtual ~Task() = default;
    virtual void run() = 0;
};

class ThreadPool
{
public:
    static void WorkerThread(ThreadPool *master);

    ThreadPool(size_t thread_count = 0);
    ~ThreadPool();

    void parallelFor(size_t width, size_t height, const std::function<void(size_t, size_t)> &lambda);
    void wait() const;

    void addTask(Task *task);
    Task *getTask();

private:
    std::atomic<int> alive;
    std::atomic<int> pengding_task_count;
    std::vector<std::thread> threads;
    std::queue<Task *> tasks;
    SpinLock spin_lock{};
};

extern ThreadPool thread_pool;