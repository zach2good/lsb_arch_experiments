#pragma once

#include "common.h"
#include "thread_pool.h"

// scheduler.h
/*
Example usage:

    // Defined tasks
    auto someTask() -> task<int>
    {
        // Some heavy computation
        co_return 42;
    }

    co_await someTask();

    // Inline
    int x = co_await scheduler_.postToWorkerThread([]() -> int
    {
        // Some heavy computation
        co_return 42;
    });
*/
class Scheduler final
{
public:
    explicit Scheduler(std::size_t numThreads)
    : threadPool_(numThreads)
    {
    }

    ~Scheduler() = default;

    void postToMainThread()
    {
    }

    void postToWorkerThread()
    {
    }

    void waitForAllTasks() // blocking
    {
    }

private:
    ThreadPool threadPool_;
};