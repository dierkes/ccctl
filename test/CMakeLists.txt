cmake_minimum_required(VERSION 2.8)

project(ccctl_gtest)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11" )

include_directories(
    "../include"
	"../gtest/include"
)

file(GLOB source_files
    "*.cpp"
)

add_executable(ccctl_gtest ${source_files})

target_link_libraries(ccctl_gtest
    gtest_main
    gtest
)
