#pragma once

// networking.h
// Communication with connected clients.
// Uses its own thread, communicates with other services by double lockless queues and events.
class Networking final
{
public:
    Networking(Scheduler& scheduler)
    : scheduler_(scheduler)
    {
    }

    DELETE_COPY_AND_MOVE(Networking);

    Scheduler& scheduler_;
};
