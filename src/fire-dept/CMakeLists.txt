cmake_minimum_required(VERSION 3.7)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(./../core core)
include_directories(${SHARED_LIB_INCLUDE_DIR} ./inc)

set(SOURCE_FILES src/main.cpp)
add_executable(fire-dept ${SOURCE_FILES})
target_link_libraries(fire-dept core)
