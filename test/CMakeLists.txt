cmake_minimum_required(VERSION 3.12)
include_directories(googletest-master/googletest/include googletest-master/googletest)
include_directories(../src)

add_subdirectory(googletest-master)
add_subdirectory(spl_SSA_test)