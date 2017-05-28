cmake_minimum_required(VERSION 3.7)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z")

add_subdirectory(./../core core)
include_directories(${SHARED_LIB_INCLUDE_DIR} ./inc)

set(SOURCE_FILES
        src/main.cpp
        src/Central.cpp inc/Central.h
        inc/CentralConfig.h inc/AlertAggregator.h src/AlertAggregator.cpp)
add_executable(central ${SOURCE_FILES})
target_link_libraries(central core pthread)
