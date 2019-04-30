cmake_minimum_required(VERSION 3.12)
project(SPLCompiler CXX)
set(CXXDEBUG "-g -Wall")
set(CXXSTD "-std=c++14")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 ${CXXDEBUG} ${CXXSTD}")

find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

BISON_TARGET(parser
        ${CMAKE_CURRENT_SOURCE_DIR}/src/spl_parser.y
        ${CMAKE_CURRENT_SOURCE_DIR}/src/spl_parser.tab.cc)
FLEX_TARGET(lexer
        ${CMAKE_CURRENT_SOURCE_DIR}/src/spl_lexer.l
        ${CMAKE_CURRENT_SOURCE_DIR}/src/spl_lexer.yy.cc)
ADD_FLEX_BISON_DEPENDENCY(lexer parser)

add_executable(SPLCompiler
        src/main.cpp
        src/spl_driver.cpp
        ${FLEX_lexer_OUTPUTS}
        ${BISON_parser_OUTPUTS}
        src/spl_ast.cpp src/spl_symtab.cpp src/spl_compiler.cpp src/spl_exception.cpp src/spl_exception.hpp src/consoleColor.hpp src/SPL_IR.cpp src/SPL_IR.hpp src/spl_SSA.cpp src/spl_SSA.hpp src/spl_codeGen.cpp src/spl_codeGen.h)

add_subdirectory(test)

target_include_directories(SPLCompiler PUBLIC include)

target_include_directories(SPLCompiler
        PUBLIC
        .
        ${CMAKE_CURRENT_SOURCE_DIR})