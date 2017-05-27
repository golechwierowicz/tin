cmake_minimum_required(VERSION 3.7)
project(tin)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(src/core)
add_subdirectory(src/sensor)
add_subdirectory(src/control-center)
add_subdirectory(src/fire-dept)
add_subdirectory(src/central)
