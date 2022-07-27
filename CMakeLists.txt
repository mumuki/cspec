cmake_minimum_required(VERSION 3.5)
project(cspecs)

set(SOURCE_FILES
        cspecs/cspec.h
        cspecs/cspec.c)

add_library(cspecs SHARED ${SOURCE_FILES})
set_target_properties(cspecs PROPERTIES PUBLIC_HEADER cspecs/cspec.h)

install(TARGETS cspecs
        LIBRARY DESTINATION lib
        PUBLIC_HEADER DESTINATION include/cspecs)
