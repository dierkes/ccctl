cmake_minimum_required(VERSION 2.8)

project(CCCTL_Test)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11" )

include_directories(
    "../include"
)

file(GLOB source_files
    "*.cpp"
)

add_executable(gtest_ccctl ${source_files})

target_link_libraries(gtest_ccctl
    gtest_main
    gtest
)
