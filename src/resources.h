#pragma once

#include "common.h"

#include "resource_pool.h"

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