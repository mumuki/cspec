cmake_minimum_required(VERSION 3.5)
project(cspecs)

set(SOURCE_FILES
        cspecs/cspec.h
        cspecs/cspec.c)

add_library(cspecs SHARED ${SOURCE_FILES})

install(TARGETS cspecs
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
