#pragma once

#include "application.h"
#include <chrono>
#include <coroutine>
#include <cstdint>
#include <string>
#include <thread>
#include <vector>

// Cross-platform pragma push/disable/pop


// TODO
// #pragma warning(push)
// #pragma warning(disable : 26495)
#include "tracy.h"
// #pragma warning(pop)

//
// Macros/ifdefs
//

#define DEFAULT_COPY(TypeName)                      \
    TypeName(const TypeName&)            = default; \
    TypeName& operator=(const TypeName&) = default;

#define DEFAULT_MOVE(TypeName)                 \
    TypeName(TypeName&&)            = default; \
    TypeName& operator=(TypeName&&) = default;

#define DEFAULT_COPY_AND_MOVE(TypeName) \
    DEFAULT_COPY(TypeName);             \
    DEFAULT_MOVE(TypeName);

#define DELETE_COPY(TypeName)                      \
    TypeName(const TypeName&)            = delete; \
    TypeName& operator=(const TypeName&) = delete;

#define DELETE_MOVE(TypeName)                 \
    TypeName(TypeName&&)            = delete; \
    TypeName& operator=(TypeName&&) = delete;

#define DELETE_COPY_AND_MOVE(TypeName) \
    DELETE_COPY(TypeName);           \
    DELETE_MOVE(TypeName)

// https://stackoverflow.com/questions/1505582/determining-32-vs-64-bit-in-c
// Check Windows
#if _WIN32 || _WIN64
#if _WIN64
#define ENV64BIT
#else
#define ENV32BIT
#endif
#endif

// Check GCC
#if __GNUC__
#if __x86_64__ || __ppc64__
#define ENV64BIT
#else
#define ENV32BIT
#endif
#endif

// debug mode
#if defined(_DEBUG) && !defined(DEBUG)
#define DEBUG
#endif

//
// typedef/using
//

using int8  = std::int8_t;
using int16 = std::int16_t;
using int32 = std::int32_t;
using int64 = std::int64_t;

using uint8  = std::uint8_t;
using uint16 = std::uint16_t;
using uint32 = std::uint32_t;
using uint64 = std::uint64_t;

using f32 = float;
using f64 = double;

using namespace std::literals::chrono_literals;
using server_clock = std::chrono::system_clock;
using time_point   = server_clock::time_point;
using duration     = server_clock::duration;

using hires_clock      = std::chrono::high_resolution_clock;
using hires_time_point = server_clock::time_point;
using hires_duration   = server_clock::duration;

//
// Forward declarations
//

class Application;
class Scheduler;
class Resources;
class Settings;
class Networking;
class RpcClient;
class Audit;
class MapState;
class ConsoleService;
class ThreadPool;
