cmake_minimum_required(VERSION 3.15 FATAL_ERROR)
project(lsb_new_architecture LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(cmake/CPM.cmake)

CPMAddPackage(
    NAME spdlog
    GITHUB_REPOSITORY gabime/spdlog
    GIT_TAG v1.14.1
)

set(TRACY_VERSION 0.11.0)

# Download client library
CPMAddPackage(
    NAME TracyClient
    GITHUB_REPOSITORY wolfpld/tracy
    GIT_TAG v${TRACY_VERSION}
    OPTIONS
    "TRACY_ENABLE ON"
    "TRACY_ON_DEMAND ON"
    "TRACY_CALLSTACK ON" # Value hardcoded to 4 in common/tracy.h
    "TRACY_NO_BROADCAST ON"
    "TRACY_NO_CONTEXT_SWITCH ON"
    "TRACY_NO_EXIT ON"
    "TRACY_NO_VSYNC_CAPTURE ON"
    "TRACY_NO_FRAME_IMAGE ON"
    "TRACY_LIBBACKTRACE_ELF_DYNLOAD_SUPPORT ON"
)

message(STATUS "Downloading Tracy client")
file(DOWNLOAD
        https://github.com/wolfpld/tracy/releases/download/v${TRACY_VERSION}/windows-${TRACY_VERSION}.zip
        ${CMAKE_SOURCE_DIR}/tracy.tar.gz
        TIMEOUT 60
)
execute_process(COMMAND "${CMAKE_COMMAND}" -E
        tar xvf "${CMAKE_SOURCE_DIR}/tracy.tar.gz"
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/
)

set(SOURCES
    src/main.cpp
)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC src/)
# target_compile_options(${PROJECT_NAME} PUBLIC /Wall)
target_link_libraries(${PROJECT_NAME} PUBLIC spdlog::spdlog TracyClient)
set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/INCREMENTAL:NO")