#pragma once

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

    // Audit task

    Scheduler& scheduler_;
    Resources& resources_;
};
