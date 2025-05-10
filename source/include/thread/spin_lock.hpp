#pragma once

#include <atomic>
#include <thread>

class SpinLock
{
public:
    void acquire()
    {
        while (m_flag.test_and_set(std::memory_order_acquire))
        {
            std::this_thread::yield();
        }
    }

    void release()
    {
        m_flag.clear(std::memory_order_release);
    }

private:
    std::atomic_flag m_flag = ATOMIC_FLAG_INIT;
};

class Guard
{
public:
    explicit Guard(SpinLock &lock)
        : m_lock(lock)
    {
        m_lock.acquire();
    }

    ~Guard()
    {
        m_lock.release();
    }

private:
    SpinLock &m_lock;
};
