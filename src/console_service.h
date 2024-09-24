#pragma once

#include "common.h"
#include "mutex_guarded.h"

// console_service.h
class ConsoleService final
{
public:
    ConsoleService(Application& app)
    : app_(app)
    {
        std::ignore = testInt_.read();
    }

    DELETE_COPY_AND_MOVE(ConsoleService);

    Application& app_;
    MutexGuarded<int> testInt_;
};
