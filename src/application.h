#pragma once

#include "common.h"

#include "scheduler.h"
#include "resources.h"
#include "settings.h"
#include "networking.h"
#include "rpc_client.h"
#include "audit.h"
#include "map_state.h"
#include "console_service.h"

//
// Forward declarations
//

class Scheduler;
class Resources;
class Settings;
class Networking;
class RpcClient;
class Audit;
class MapState;
class ConsoleService;

// application.h
class Application final
{
public:
    Application()
    // Scheduler runs the whole rest of the program via its own thread pool.
    // The main thread is only used for the main loop, therefor there must always be at least 1 worker thread.
    : scheduler_(/*numThreads*/ std::max(1U, std::thread::hardware_concurrency() - 1U))
    , resources_(scheduler_)
    , settings_(scheduler_, resources_)
    , networking_(scheduler_)
    , rpcClient_(scheduler_)
    , audit_(scheduler_, resources_)
    , mapState_(scheduler_, resources_, networking_, rpcClient_, audit_)
    , consoleService_(*this)
    {
        // Cue up any additional tasks needed to complete before the main loop

        // Wait for all tasks to complete
        scheduler_.waitForAllTasks();
    }

    ~Application()
    {
        // Teardown
    }

    DELETE_COPY_AND_MOVE(Application);

    void run()
    {
        // Everything is ready to go, start the main loop
        while (running_)
        {
            // Scheduler things
            scheduler_.postToMainThread(/**/);
        }
    }

private:
    // What does the app need to run?

    std::atomic<bool> running_{ true };

    Scheduler      scheduler_;
    Resources      resources_;
    Settings       settings_;
    Networking     networking_;
    RpcClient      rpcClient_;
    Audit          audit_;
    MapState       mapState_;
    ConsoleService consoleService_;
};
