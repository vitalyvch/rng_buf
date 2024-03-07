
##
# @author Vitalii Chernookyi
#                           
# @copyright BSD

##
# @author Guillermo Rodriguez
#                            
# modified it to be a shared lib and dropped the testing stuff
# @copyright BSD

cmake_minimum_required(VERSION 3.5.0)
include(CheckCSourceCompiles)

add_library(rng_buf SHARED rng_buf.c rng_buf.h)
set_target_properties(rng_buf PROPERTIES
    PREFIX ""
    C_STANDARD 99
    COMPILE_OPTIONS "-O2;-pthread;-Wall;-Wextra;-std=gnu99"
    LINK_FLAGS "-O2 -pthread")

target_include_directories(rng_buf PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_library(rng_buf_mt SHARED rng_buf_mt.c rng_buf.h)
set_target_properties(rng_buf_mt PROPERTIES
    PREFIX ""
    C_STANDARD 99
    COMPILE_OPTIONS "-O2;-pthread;-Wall;-Wextra;-std=gnu99"
    LINK_FLAGS "-O2 -pthread")

target_include_directories(rng_buf_mt PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

