#pragma once

#include "common.h"

#include "resource_pool.h"

class LuaWrapper final
{
};

class SQLWrapper final
{
};

// navmesh.h
// A wrapper around dtNavMeshQuery and the underlying navmesh files.
// If we handle dtNavMeshQuery as a pooled resource, we can access
// different navmeshes from different threads.
class NavmeshWrapper final
{
};

// resources.h
class Resources final
{
public:
    Resources(Scheduler& scheduler)
    : scheduler_(scheduler)
    , luaPool_(1)
    , sqlPool_(4)
    , navmeshPool_(4)
    {
    }

    DELETE_COPY_AND_MOVE(Resources);

    Scheduler& scheduler_;

    ResourcePool<LuaWrapper>     luaPool_;
    ResourcePool<SQLWrapper>     sqlPool_;
    ResourcePool<NavmeshWrapper> navmeshPool_;
};