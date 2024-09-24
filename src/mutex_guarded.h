#pragma once

#include "common.h"

#include <mutex>
#include <shared_mutex>

// https://www.reddit.com/r/cpp/comments/p132c7/comment/h8b8nml/?share_id=-NRyj9iRw5TqSi4Mm381j
template <
    class T,
    class Mutex                            = std::mutex,
    template <typename...> typename WriteLock = std::unique_lock,
    template <typename...> typename ReadLock = std::unique_lock>
struct MutexGuarded
{
    MutexGuarded()  = default;
    ~MutexGuarded() = default;

    explicit MutexGuarded(T in)
    : target(std::move(in))
    {
    }

    DELETE_COPY_AND_MOVE(MutexGuarded<T>);

    auto read(auto f) const
    {
        auto l = lock();
        LockMark(mutex);
        return f(target);
    }

    auto read() const
    {
        // TODO: Test this is safe
        return read([&](const T& obj) { return obj; });
    }

    auto write(auto f)
    {
        auto l = lock();
        LockMark(mutex);
        return f(target);
    }

private:
    mutable TracyLockable(Mutex, mutex);

    T target;

    auto lock() const
    {
        return ReadLock<LockableBase(Mutex)>(mutex);
    }

    auto lock()
    {
        return WriteLock<LockableBase(Mutex)>(mutex);
    }
};

template <class T>
using shared_guarded = MutexGuarded<T, std::shared_mutex, std::shared_lock>;
