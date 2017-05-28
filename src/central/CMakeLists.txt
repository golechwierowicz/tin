cmake_minimum_required(VERSION 3.7)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(./../core core)
include_directories(${SHARED_LIB_INCLUDE_DIR} ./inc)

set(SOURCE_FILES
        src/Central.cpp inc/Central.h
        inc/CentralConfig.h inc/AlertAggregator.h src/AlertAggregator.cpp)
add_executable(central ${SOURCE_FILES} src/main.cpp)
target_link_libraries(central core pthread)

enable_testing()
find_package(Boost COMPONENTS unit_test_framework REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

set(TEST_FILES ${SOURCE_FILES}
        test/AlertAggregator.test.cpp test/Central.test.cpp)
add_executable(central_test ${TEST_FILES})
target_link_libraries(central_test core pthread ${Boost_LIBRARIES})

set(IT_TEST_FILES ${SOURCE_FILES}
        it-test/Central.it-test.cpp it-test/AlertAggregator.it-test.cpp)
add_executable(central_it_test ${IT_TEST_FILES})
target_link_libraries(central_it_test core pthread ${Boost_LIBRARIES})
