#include "resource_pool.h"

#include <thread>

class LuaWrapper final
{
    struct FakeLuaState
    {
    };
    FakeLuaState luaState_;
};

class SQLWrapper final
{
    struct FakeSQLConnection
    {
    };
    FakeSQLConnection sqlConnection_;
};

// navmesh.h
// A wrapper around dtNavMeshQuery and the underlying navmesh files.
// If we handle dtNavMeshQuery as a pooled resource, we can access
// different navmeshes from different threads.
class NavmeshWrapper final
{
    struct FakeNavmesh
    {
    };
    FakeNavmesh navmesh_;
};

// thread_pool.h
// A thread pool that can be used to run tasks on worker threads.
class ThreadPool final
{
public:
    explicit ThreadPool(std::size_t numThreads)
    {
        // Ensure numThreads is >= 1
    }

    ~ThreadPool() = default;

private:
    std::vector<std::jthread> threads_;
};

// scheduler.h
/*
Example usage:

    // Defined tasks
    auto someTask() -> task<int>
    {
        // Some heavy computation
        co_return 42;
    }

    co_await someTask();

    // Inline
    int x = co_await scheduler_.postToWorkerThread([]() -> int
    {
        // Some heavy computation
        co_return 42;
    });
*/
class Scheduler final
{
public:
    explicit Scheduler(std::size_t numThreads)
    : threadPool_(numThreads)
    {
    }

    ~Scheduler() = default;

    void postToMainThread()
    {
    }

    void postToWorkerThread()
    {
    }

    void waitForAllTasks() // blocking
    {
    }

private:
    ThreadPool threadPool_;
};

// resources.h
struct Resources final
{
    Resources(Scheduler& scheduler)
    : scheduler_(scheduler)
    , luaPool_(1)
    , sqlPool_(4)
    , navmeshPool_(4)
    {
    }

    ResourcePool<LuaWrapper>     luaPool_;
    ResourcePool<SQLWrapper>     sqlPool_;
    ResourcePool<NavmeshWrapper> navmeshPool_;

    Scheduler& scheduler_;
};

// settings.h
class Settings final
{
public:
    Settings(Scheduler& scheduler, Resources& resources)
    : scheduler_(scheduler)
    , resources_(resources)
    {
        // Settings are linked from Lua, so we need access to resources_.luaPool_
    }

    Scheduler& scheduler_;
    Resources& resources_;
};

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

    Scheduler& scheduler_;
};

// rpc_client.h
// Communication with other server executables
class RpcClient final
{
public:
    RpcClient(Scheduler& scheduler)
    : scheduler_(scheduler)
    {
    }

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

// map_state.h
// The meat and potatoes of the zone, entity, and tick logic
class MapState final
{
public:
    MapState(
        Scheduler&  scheduler,
        Resources&  resources,
        Networking& networking,
        RpcClient&  rpcClient,
        Audit&      audit)
    : scheduler_(scheduler)
    , resources_(resources)
    , networking_(networking)
    , rpcClient_(rpcClient)
    , audit_(audit)
    {
    }

    auto luaTestTask() -> task<void>
    {
        auto [_, lua] = this->resources_.luaPool_.getHandleAndRef()
        lua["print"]("Oh boy oh boy!");
    }

    auto tick() -> task<void>
    {
        for (auto& zone : zones_)
        {
            co_await zone.handleNetworkMessages();
            co_await zone.tickZonesAndEntities();
        }

        co_await luaTestTask();

        scheduler_.waitForAllTasks();
    }

    struct Zone
    {
        auto handleNetworkMessages() -> task<void>
        {
            // Network message handling logic
            // network_.receiveMessage(); // etc.
        }

        auto tickZonesAndEntities() -> task<void>
        {
            // Zone tick logic
        }

        struct Entity
        {
        };
        std::vector<Entity> entities_;
    };
    std::vector<Zone> zones_;

    Scheduler&  scheduler_;
    Resources&  resources_;
    Networking& networking_;
    RpcClient&  rpcClient_;
    Audit&      audit_;
};

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

    void run()
    {
        // Everything is ready to go, start the main loop
        while (running_)
        {
            // Scheduler things
            scheduler_.postToMainThread(mapState_.tick());
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

// main.cpp
int main()
{
    auto app = std::make_unique<Application>();
    app->run();
    return 0;
}
