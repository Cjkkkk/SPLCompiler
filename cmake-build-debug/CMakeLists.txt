set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-deprecated-register")
project(SPL_Compiler CXX)
cmake_minimum_required(VERSION 3.12.0)

find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

BISON_TARGET(spl_parser
        ../spl_parser.y
        ${CMAKE_CURRENT_BINARY_DIR}/spl_parser.tab.c)
FLEX_TARGET(spl_lexer
        ../spl_lexer.l
        ${CMAKE_CURRENT_BINARY_DIR}/lex.yy.cc)
ADD_FLEX_BISON_DEPENDENCY(spl_lexer spl_parser)

add_executable(SPL_Compiler
        ../main.cpp
        ../spl_driver.cpp
        ${FLEX_spl_lexer_OUTPUTS}
        ${BISON_spl_parser_OUTPUTS}
        )
target_include_directories(SPL_Compiler
        PRIVATE
        ..
        ${CMAKE_CURRENT_BINARY_DIR})

set_property(TARGET SPL_Compiler PROPERTY CXX_STANDARD 14)
#enable_testing()
#add_test(NAME basic
#        COMMAND ${CMAKE_SOURCE_DIR}/test/test0.pl
#        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})