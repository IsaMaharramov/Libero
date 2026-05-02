cmake_minimum_required(VERSION 3.10)
project(liberoscroller)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(liberoscroller main.cpp)

target_link_libraries(liberoscroller ${OpenCV_LIBS} User32.lib)