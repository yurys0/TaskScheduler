cmake_minimum_required(VERSION 3.28)
project(labwork10_yurys0)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)
include_directories(tests)

include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(runUnitTests
        tests/tests.cpp
        include/TTask.hpp
        include/TTaskScheduler.hpp
)

target_link_libraries(runUnitTests gtest gtest_main)

target_include_directories(runUnitTests PRIVATE ${googletest_SOURCE_DIR}/include)

include(GoogleTest)
gtest_discover_tests(runUnitTests)

add_executable(labwork10_yurys0
        main.cpp
        include/TTaskScheduler.hpp
        include/TTask.hpp
        include/TFunction.hpp
)
