cmake_minimum_required(VERSION 3.7)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z")

add_subdirectory(./../core core)
include_directories(${SHARED_LIB_INCLUDE_DIR} ./inc ${CONFIG++_INCLUDE_DIR})

set(SOURCE_FILES
        src/main.cpp
        src/ControlCenter.cpp inc/ControlCenter.h
        )
add_executable(control-center ${SOURCE_FILES})
target_link_libraries(control-center core ${CONFIG++_LIBRARY})
