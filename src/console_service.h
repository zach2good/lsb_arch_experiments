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

    Application& app_;
};
