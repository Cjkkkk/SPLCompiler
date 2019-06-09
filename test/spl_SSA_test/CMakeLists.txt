cmake_minimum_required(VERSION 3.12)
project(spl_SSA_test)

add_executable(spl_SSA_test spl_SSA_test.cpp ../../src/spl_compiler.cpp ../../src/spl_SSA.cpp)

target_link_libraries(spl_SSA_test gtest gtest_main)