cmake_minimum_required(VERSION 2.8)

project(ccctl_gtest)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11" )

include_directories(
    "../include"
	"../gtest/include"
)

#file(GLOB source_files
#    "*.cpp"
#)

set(source_files
    utils.cpp
    gTest_cUniqueID.cpp
    gTest_Alignment.cpp
    gTest_Array.cpp
    gTest_Storage.cpp
    gTest_PodVector.cpp
    gTest_PodDeque.cpp
    gTest_PodList.cpp
    gTest_ConsistentVector.cpp
    gTest_ConsistentDeque.cpp
    gTest_ConsistentList.cpp
    gTest_FixedVector.cpp
    gTest_FixedDeque.cpp
    gTest_FixedList.cpp
    gTest_StaticList.cpp
)

add_executable(ccctl_gtest ${source_files})

target_link_libraries(ccctl_gtest
    gtest_main
    gtest
)
