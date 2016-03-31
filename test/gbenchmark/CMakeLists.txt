cmake_minimum_required(VERSION 2.8)

project(ccctl_gbenchmark)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11" )

include_directories(
    "../../include"
    "../include"
    "../../gbenchmark/include"
)

#file(GLOB source_files
#    "*.cpp"
#)

#set(source_files
#    gbenchmark_Vector.cpp
#)

#set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -H")
#set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -o3")
#add_cxx_compiler_flag(-O3)

macro(compile_benchmark_test name)
  add_executable(${name} "${name}.cpp")
  target_link_libraries(${name} benchmark ${CMAKE_THREAD_LIBS_INIT})
endmacro(compile_benchmark_test)

compile_benchmark_test(gbenchmark_Vector)


#add_executable(ccctl_gbenchmark ${source_files})

#target_link_libraries(ccctl_gbenchmark
#    benchmark
#)