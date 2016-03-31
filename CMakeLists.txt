cmake_minimum_required(VERSION 2.8)

project(ccctl)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11" )

add_subdirectory("gtest")

add_subdirectory("gbenchmark")

add_subdirectory("test")

set(CMAKE_BINARY_DIR "bin")
add_subdirectory("celero")
