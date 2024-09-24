#pragma once

#include "common.h"

#include <condition_variable>
#include <iostream>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

template <typename T>
class ResourceHandle;

enum class ResourcePoolInit
{
    Eager,

    // TODO: Ensure lazily made resources are created on the same thread as the pool
    // Lazy,
};

template <typename T>
class ResourcePool
{
public:
    ResourcePool(size_t poolSize, ResourcePoolInit init = ResourcePoolInit::Eager)
    : poolSize_(poolSize)
    , init_(init)
    {
        resourcesPool_.reserve(poolSize_);

        if (init_ == ResourcePoolInit::Eager)
        {
            while (resourcesPool_.size() < poolSize)
            {
                auto resource = std::make_unique<T>();
                resourcesPool_.emplace_back(std::move(resource), false);
            }
        }
        else
        {
            // Always create at least 1 Resource eagerly
            auto resource = std::make_unique<T>();
            resourcesPool_.emplace_back(std::move(resource), false);
        }
    }

    DELETE_COPY_AND_MOVE(ResourcePool<T>);

    auto getHandle() -> ResourceHandle<T>
    {
        std::unique_lock<std::mutex> lock(mutex_);

        // NOTE: This time shouldn't be configurable, nor should this ever be turned off.
        //     : If we reach a point where we've been waiting 3 seconds for a resource on
        //     : any thread, especially the main thread, we need to terminate - we're in
        //     : deadlock or some other fail state.
        // clang-format off
        constexpr auto timeout = std::chrono::seconds(3);
        if (!condition_.wait_for(lock, timeout, [this]() -> bool { return resourcesAvailable(); }))
        {
            // NOTE: Resource information and stack information will be included in the exception.
            throw std::runtime_error("Timed-out trying to acquire a resource!");
        }
        // clang-format on

        /*
        // Lazy-create up to poolSize_ if allowed
        if (init_ == ResourcePoolInit::Lazy && resourcesPool_.size() < poolSize_)
        {
            // TODO: This creates T on the calling thread, not the thread the pool lives on!
            auto resource = std::make_unique<T>();
            resourcesPool_.emplace_back(std::move(resource), true);
            return ResourceHandle<T>(*this, *resourcesPool_.back().obj_);
        }
        */

        // Try to find an available resource
        for (auto& resource : resourcesPool_)
        {
            if (!resource.inUse_)
            {
                resource.inUse_ = true;
                return ResourceHandle<T>(*this, *resource.obj_);
            }
        }

        // Should never get here!
        throw std::runtime_error("Critically failed to acquire a resource!");
    }

    auto getHandleAndRef() -> std::pair<ResourceHandle<T>, T&>
    {
        auto handle = getHandle();
        T&   ref    = *handle;
        return { std::move(handle), ref };
    }

    bool resourcesAvailable() const
    {
        /*
        if (init_ == ResourcePoolInit::Lazy && resourcesPool_.size() < poolSize_)
        {
            return true;
        }
        */

        // clang-format off
        return std::any_of(resourcesPool_.begin(), resourcesPool_.end(), [](const Resource& resource)
        {
            return !resource.inUse_;
        });
        // clang-format on
    }

private:
    friend class ResourceHandle<T>;

    struct Resource
    {
        Resource(std::unique_ptr<T> resourceObj, bool inUse)
        : obj_(std::move(resourceObj))
        , inUse_(inUse)
        {
        }

        DELETE_COPY(Resource);
        DEFAULT_MOVE(Resource);

        std::unique_ptr<T> obj_{ nullptr };
        bool               inUse_{ false };
    };

    void release(const T& obj)
    {
        std::unique_lock<std::mutex> lock(mutex_);

        for (auto& resource : resourcesPool_)
        {
            if (resource.obj_.get() == std::addressof(obj))
            {
                resource.inUse_ = false;
                break;
            }
        }

        condition_.notify_one();
    }

    size_t                  poolSize_;
    ResourcePoolInit        init_;
    std::vector<Resource>   resourcesPool_;
    mutable std::mutex      mutex_;
    std::condition_variable condition_;
};

template <typename T>
class ResourceHandle
{
public:
    ResourceHandle(ResourcePool<T>& pool, T& resource)
    : pool_(pool)
    , resource_(resource)
    {
    }

    ~ResourceHandle()
    {
        pool_.release(resource_);
    }

    const T* operator->() const
    {
        return &resource_;
    }

    T* operator->()
    {
        return &resource_;
    }

    const T& operator*() const
    {
        return resource_;
    }

    T& operator*()
    {
        return resource_;
    }

private:
    ResourcePool<T>& pool_;
    T&               resource_;
};
