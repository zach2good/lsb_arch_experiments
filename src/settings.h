#pragma once

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

    DELETE_COPY_AND_MOVE(Settings);

    Scheduler& scheduler_;
    Resources& resources_;
};