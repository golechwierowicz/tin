cmake_minimum_required(VERSION 3.7)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z")

add_subdirectory(./../core core)
include_directories(${SHARED_LIB_INCLUDE_DIR} ./inc ${CONFIG++_INCLUDE_DIR})

set(SOURCE_FILES
        src/Central.cpp inc/Central.h
        inc/CentralConfig.h inc/AlertAggregator.h src/AlertAggregator.cpp)
add_executable(central ${SOURCE_FILES} src/main.cpp)
target_link_libraries(central core pthread ${CONFIG++_LIBRARY})

enable_testing()
find_package(Boost COMPONENTS unit_test_framework REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

set(TEST_FILES ${SOURCE_FILES}
        test/AlertAggregator.test.cpp test/module.test.cpp)
add_executable(central_test ${TEST_FILES})
target_link_libraries(central_test core pthread ${Boost_LIBRARIES} ${CONFIG++_LIBRARY})

set(IT_TEST_FILES ${SOURCE_FILES}
        it-test/module.it-test.cpp it-test/AlertAggregator.it-test.cpp it-test/Central.it-test.cpp)
add_executable(central_it_test ${IT_TEST_FILES})
target_link_libraries(central_it_test core pthread ${Boost_LIBRARIES} ${CONFIG++_LIBRARY})
