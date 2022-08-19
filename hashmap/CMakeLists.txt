cmake_minimum_required(VERSION 3.6.0)

project(hashmap)

set( CMAKE_CXX_STANDARD 17 )

add_executable(
    hashmap
    src/hashmap.c
    src/main.c
)

target_include_directories(hashmap PUBLIC include/)

# gtest
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)

enable_testing()

add_executable(
    tests
    src/hashmap.c
    test/src/test_hashmap.cc
)

target_link_libraries(
    tests
    gtest_main
    gtest
    pthread
)

target_include_directories(
    tests PUBLIC
    include/
    test/include/
)

include(GoogleTest)
gtest_discover_tests(tests)
