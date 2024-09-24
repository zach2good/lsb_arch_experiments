#pragma once

#include "common.h"

// rpc_client.h
// Communication with other server executables
class RpcClient final
{
public:
    RpcClient(Scheduler& scheduler)
    : scheduler_(scheduler)
    {
    }

    DELETE_COPY_AND_MOVE(RpcClient);

    // Populated with an internal impl of auto-generated rpc calls
    // from our own IDL.
    // Rpc calls are built on top of coroutines.
    // In a coroutine:
    // - execute sending the ZMQ message, suspend
    // - we get the response, look up the relevant coroutine, resume
    // The waiting probably shouldn't be blocking, but it should send an id that will then be returned and used to
    // look up the "mailbox" that's waiting for the response.
    // If the message is a void return type, waiting for the response is skipped.

    Scheduler& scheduler_;
};
