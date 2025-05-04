cmake_minimum_required(VERSION 3.16)
project(CLIRuntime VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Platform-specific settings
if(WIN32)
    add_definitions(-D_WIN32_WINNT=0x0601)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_WIN32_WINNT=0x0601")
endif()

# External dependencies
include(FetchContent)

# spdlog
FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.14.1
)
FetchContent_MakeAvailable(spdlog)

# googletest
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.11.0
)
FetchContent_MakeAvailable(googletest)

# CLI11
FetchContent_Declare(
    CLI11
    GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
    GIT_TAG v2.3.2
)
FetchContent_MakeAvailable(CLI11)

# Main executable
add_executable(cliruntime
    src/main.cpp
    src/common/logger.cpp
    src/common/config.cpp
    src/client/client.cpp
    src/server/server.cpp
    src/server/command_handler.cpp
)

target_include_directories(cliruntime PRIVATE
    src
)

target_link_libraries(cliruntime PRIVATE
    spdlog::spdlog
    CLI11::CLI11
)

if(WIN32)
    target_link_libraries(cliruntime PRIVATE ws2_32)
endif()

# Tests
enable_testing()
add_executable(cliruntime_tests
    tests/test_main.cpp
    tests/test_client.cpp
    tests/test_server.cpp
    src/common/logger.cpp
    src/common/config.cpp
    src/client/client.cpp
    src/server/server.cpp
    src/server/command_handler.cpp
)

target_include_directories(cliruntime_tests PRIVATE
    src
)

target_link_libraries(cliruntime_tests PRIVATE
    gtest
    gtest_main
    spdlog::spdlog
    CLI11::CLI11
)

if(WIN32)
    target_link_libraries(cliruntime_tests PRIVATE ws2_32)
endif()

add_test(NAME CLIRuntimeTests COMMAND cliruntime_tests)

# Packaging (optional)
include(CPack)
set(CPACK_PACKAGE_NAME "CLIRuntime")
set(CPACK_GENERATOR "ZIP")
set(CPACK_PACKAGE_MAINTAINER "CLIRuntime Team")