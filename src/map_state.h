#pragma once

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

    DELETE_COPY_AND_MOVE(MapState);

    auto luaTestTask() -> void // task<void>
    {
        auto [_, lua] = this->resources_.luaPool_.getHandleAndRef();
        //lua["print"]("Oh boy oh boy!");
    }

    auto tick() -> void //task<void>
    {
        for (auto& zone : zones_)
        {
            // co_await zone.handleNetworkMessages();
            // co_await zone.tickZonesAndEntities();
        }

        // co_await luaTestTask();

        scheduler_.waitForAllTasks();
    }

    struct Zone
    {
        auto handleNetworkMessages() -> void // task<void>
        {
            // Network message handling logic
            // network_.receiveMessage(); // etc.
        }

        auto tickZonesAndEntities() -> void // task<void>
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
