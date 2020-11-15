cmake_minimum_required(VERSION 3.13)
set(SIG8_VERSION 0.1.0)
project(sig8 VERSION ${SIG8_VERSION} DESCRIPTION "Retro game library")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")

add_subdirectory(src)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    # Only compile examples if building the library itself
    add_subdirectory(examples)
endif()
