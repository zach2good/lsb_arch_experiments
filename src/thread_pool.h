#pragma once

#include "common.h"

// thread_pool.h
// A thread pool that can be used to run tasks on worker threads.
class ThreadPool final
{
public:
    explicit ThreadPool(std::size_t numThreads)
    {
        // Ensure numThreads is >= 1

        std::ignore = numThreads;
    }

    ~ThreadPool() = default;

    DELETE_COPY_AND_MOVE(ThreadPool);

private:
    // TODO: nonstd::jthread
    std::vector<std::thread> threads_;
};