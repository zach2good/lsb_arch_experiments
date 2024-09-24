#pragma once

#include "common.h"

// audit.h
// Writing to db audit tables asynchonously
class Audit final
{
public:
    Audit(Scheduler& scheduler, Resources& resources)
    : scheduler_(scheduler)
    , resources_(resources)
    {
    }

    DELETE_COPY_AND_MOVE(Audit);

    // Audit task

    Scheduler& scheduler_;
    Resources& resources_;
};
