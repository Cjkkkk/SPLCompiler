cmake_minimum_required(VERSION 3.12)
project(SPLCompiler)

add_custom_target(SPLCompiler
        ALL
        COMMAND make all
        WORKING_DIRECTORY ..
        )