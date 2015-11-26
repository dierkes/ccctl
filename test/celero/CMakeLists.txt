cmake_minimum_required(VERSION 2.8)

project(ccctl_celero)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11" )

include_directories(
    "../../include"
	"../../celero/include"
)

set(source_files
	celero_main.cpp
	celero_test.cpp
	celero_fill.cpp
)

add_executable(ccctl_celero ${source_files})

target_link_libraries(ccctl_celero
    celero
)
