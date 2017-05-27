cmake_minimum_required(VERSION 3.7)

set(CMAKE_CXX_STANDARD 11)

set(SHARED_LIB_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/inc)
set(SHARED_LIB_INCLUDE_DIR ${SHARED_LIB_INCLUDE_DIR} PARENT_SCOPE)

if (NOT TARGET core) # run once
    include_directories(${SHARED_LIB_INCLUDE_DIR})
    set(SOURCE_FILES
            inc/Logger.h
            src/Serializer.cpp inc/Serializer.h
            src/Deserializer.cpp inc/Deserializer.h
            src/UdpConnection.cpp inc/UdpConnection.h
            src/errorhandling.cpp inc/errorhandling.h
            src/blocks/BlockReader.cpp inc/blocks/BlockReader.h
            inc/blocks/AbstractBlock.h
            src/blocks/DebugBlock.cpp inc/blocks/DebugBlock.h
            src/blocks/CntSensorConfigBlock.cpp inc/blocks/CntSensorConfigBlock.h src/blocks/RequestConfigBlock.cpp inc/blocks/RequestConfigBlock.h src/blocks/PingBlock.cpp inc/blocks/PingBlock.h src/blocks/SensorMeasurementBlock.cpp inc/blocks/SensorMeasurementBlock.h)

    find_package(Config++ REQUIRED)
    include_directories(${Config++_INCLUDE_DIRS})
    set(LIBS ${LIBS} ${Config++_LIBRARIES})
    add_library(core STATIC ${SOURCE_FILES})

    # testing
    enable_testing()
    find_package(Boost COMPONENTS unit_test_framework REQUIRED)
    include_directories(${Boost_INCLUDE_DIRS})
    set(TEST_FILES ${SOURCE_FILES}
            test/Core.test.cpp
            test/Serializer.test.cpp
            test/Deserializer.test.cpp
            test/BlockReader.test.cpp)
    add_executable(core_test ${TEST_FILES})
    target_link_libraries(core_test ${Boost_LIBRARIES})
endif ()
