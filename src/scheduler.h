#pragma once

#include "task.h"
#include <cstddef>

class Scheduler
{
public:
    explicit Scheduler(std::size_t num_threads)
    {
    }

    ~Scheduler() = default;

    void post_to_main_thread(coro::task<> task1){}
    void post_to_worker_thread(coro::task<> task1){}
    void wait_for_all_tasks(){}
};
