#include <boost/test/unit_test.hpp>
#include "AlertAggregator.h"
#include <iostream>
#include <thread>
#include <vector>
#include <algorithm>
#include <functional>

using namespace std;
using placeholders::_1;

namespace {
    struct SensorMockData {
        uint16_t latitude;
        uint16_t longitude;

        vector<SensorMeasurementBlock> measurements;
    };

    const SensorMeasurementBlock smoke(BlockType::smoke_read, 1.0);
    const SensorMeasurementBlock ir(BlockType::ir_read, 0.6);
    const SensorMeasurementBlock temperature(BlockType::temp_read, 180.0);

    const vector<SensorMockData> sensor_mocks = {
            {
                    10, 10,
                    {smoke, ir}
            },
            {
                    12, 13,
                    {smoke}
            },
            {
                    15, 15,
                    {ir, temperature}
            },
            {
                    9, 8,
                    {smoke, temperature}
            },
            {
                    9, 10,
                    {smoke}
            },
            {
                    7, 6,
                    {smoke}
            },
            {
                    15, 16,
                    {smoke}
            }
    };
}

BOOST_AUTO_TEST_CASE(check_synchronization_with_multiple_sensors) {
    const size_t repeat_per_sensor = 5 * 1000 * 1000;
    const size_t sensor_duplication = 3;

    AlertAggregator aggregator;
    vector<thread> sensors;
    sensors.reserve(sensor_mocks.size() * sensor_duplication);

    const uint64_t start_time = static_cast<uint64_t>(time(nullptr));
    for (size_t duplicate_id = 0; duplicate_id < sensor_duplication; ++duplicate_id) {
        for (const auto &mock : sensor_mocks) {
            sensors.push_back(thread([mock, repeat_per_sensor, &aggregator]() {
                for (size_t i = 0; i < repeat_per_sensor; ++i) {
                    if (i % (1000 * 1000) == 0) {
                        const auto percent = (i * 100) / repeat_per_sensor;
                        BOOST_TEST_MESSAGE("sensor[" << mock.latitude << "; " << mock.longitude << "] iteration=" << i
                                                     << " (" << percent << "%)");
                    }

                    const SensorCommonBlock common(
                            static_cast<uint64_t>(time(nullptr)),
                            mock.latitude,
                            mock.longitude,
                            true
                    );

                    for (const auto &measurement : mock.measurements) {
                        aggregator.insert(common, measurement);
                    }
                }
            }));
        }
    }

    for_each(sensors.begin(), sensors.end(), bind(&thread::join, _1));

    auto result = aggregator.extractAlerts();

    BOOST_CHECK_EQUAL(sensor_mocks.size(), result.size());
    for (const auto &sensor_data : result) {
        BOOST_TEST_MESSAGE("Checking for latitude=" << sensor_data.get_latitude()
                                                    << "; longitude=" << sensor_data.get_longitude());

        BOOST_CHECK_GE(start_time, sensor_data.get_timestamp());

        auto it = find_if(sensor_mocks.begin(), sensor_mocks.end(), [&sensor_data](const SensorMockData &mock) {
            return mock.latitude == sensor_data.get_latitude() &&
                    mock.longitude == sensor_data.get_longitude();
        });

        BOOST_CHECK_MESSAGE(sensor_mocks.end() != it, "couldn't find corresponding sensor");

        BOOST_CHECK_EQUAL(
                it->measurements.size() * repeat_per_sensor * sensor_duplication,
                sensor_data.get_alerts_count()
        );
    }
}

