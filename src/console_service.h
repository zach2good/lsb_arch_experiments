#pragma once

#include "common.h"

// console_service.h
class ConsoleService final
{
public:
    ConsoleService(Application& app)
    : app_(app)
    {
    }

    DELETE_COPY_AND_MOVE(ConsoleService);

    Application& app_;
};
