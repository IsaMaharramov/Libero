cmake_minimum_required(VERSION 3.10)
project(liberoscroller)

set(CMAKE_CXX_STANDARD 17)

# Find OpenCV library
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

# Create the executable
add_executable(liberoscroller main.cpp)

# Link the libraries
target_link_libraries(liberoscroller ${OpenCV_LIBS} User32.lib)