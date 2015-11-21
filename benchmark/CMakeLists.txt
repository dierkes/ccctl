cmake_minimum_required(VERSION 2.8)

project(CCCTL_Benchmark)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11" )

include_directories(
    "../include"
	"../celero/include"
)

file(GLOB source_files
    "*.cpp"
)

add_executable(celero_ccctl ${source_files})

target_link_libraries(celero_ccctl
    celero
)
