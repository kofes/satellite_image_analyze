cmake_minimum_required(VERSION 3.6)
project(GenerateImage)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_DEBUG} -Wall -g")

set(SOURCE_EXE gen_image.cpp)

include_directories(../inc)

add_executable(${PROJECT_NAME} ${SOURCE_EXE})

file(GLOB SOURCE_LIB "../src/[M|P|I]*.cpp")

add_library(satellite SHARED ${SOURCE_LIB})

target_link_libraries(${PROJECT_NAME} satellite)
