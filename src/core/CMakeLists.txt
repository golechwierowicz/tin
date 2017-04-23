cmake_minimum_required(VERSION 3.7)

set(CMAKE_CXX_STANDARD 11)

set(SHARED_LIB_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/inc)
set(SHARED_LIB_INCLUDE_DIR ${SHARED_LIB_INCLUDE_DIR} PARENT_SCOPE)

if(NOT TARGET core)
    include_directories(${SHARED_LIB_INCLUDE_DIR})
    set(SOURCE_FILES src/Serializer.cpp)
    add_library(core STATIC ${SOURCE_FILES})
endif()
