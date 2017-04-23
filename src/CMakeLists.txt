cmake_minimum_required(VERSION 3.6)

project(satellite)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -g")

include_directories(../inc)

file(GLOB SOURCE_LIB "[M|P|I]*.cpp")

add_library(satellite SHARED ${SOURCE_LIB})
