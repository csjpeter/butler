cmake_minimum_required(VERSION 3.16)
project(libcsjp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Collect source files (excluding test directories)
file(GLOB_RECURSE CSJP_SRC
    core/*.cpp
    system/*.cpp
    container/*.cpp
    devel/*.cpp
)

# Exclude test files
foreach(file ${CSJP_SRC})
    if(file MATCHES "/test/")
        list(REMOVE_ITEM CSJP_SRC ${file})
    endif()
endforeach()

# Add include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/core
    ${CMAKE_CURRENT_SOURCE_DIR}/system
    ${CMAKE_CURRENT_SOURCE_DIR}/container
    ${CMAKE_CURRENT_SOURCE_DIR}/human
    ${CMAKE_CURRENT_SOURCE_DIR}/devel
)

# Define the static library target
add_library(libcsjp STATIC ${CSJP_SRC})

# Optionally export include directories for use in other projects
target_include_directories(libcsjp PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/core>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/system>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/container>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/human>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/devel>
)

