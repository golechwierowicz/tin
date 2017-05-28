#include <boost/test/unit_test.hpp>
#include "AlertAggregator.h"

namespace {
    const uint64_t timestamp = 1495986528ULL;
    const uint64_t timestamp_after_minute = timestamp + 60;
    const uint16_t latitude = 120;
    const uint16_t longitude = 130;

    const SensorCommonBlock common(timestamp, latitude, longitude, true);
    const SensorCommonBlock common_after_minute(timestamp_after_minute, latitude, longitude, true);
    const SensorCommonBlock common_not_alive(timestamp, latitude, longitude, false);

    const SensorMeasurementBlock smoke(BlockType::smoke_read, 1.0);
    const SensorMeasurementBlock smoke_below_threshold(BlockType::smoke_read, 0.0);
    const SensorMeasurementBlock ir(BlockType::ir_read, 0.6);
    const SensorMeasurementBlock ir_below_threshold(BlockType::ir_read, 0.0);
}

BOOST_AUTO_TEST_CASE(alert_aggregator_empty) {
    AlertAggregator aggregator;

    auto result = aggregator.extractAlerts();

    BOOST_CHECK_EQUAL(0, result.size());
}

BOOST_AUTO_TEST_CASE(alert_aggregator_several_inserts) {
    AlertAggregator aggregator;

    aggregator.insert(common, smoke);
    aggregator.insert(common, ir);
    auto initial = aggregator.extractAlerts();

    aggregator.insert(common_after_minute, smoke);
    auto after_minute = aggregator.extractAlerts();
    
    BOOST_CHECK_EQUAL(1, initial.size());
    BOOST_CHECK_EQUAL(timestamp, initial[0].get_timestamp());
    BOOST_CHECK_EQUAL(2, initial[0].get_alerts_count());
    BOOST_CHECK_EQUAL(latitude, initial[0].get_latitude());
    BOOST_CHECK_EQUAL(longitude, initial[0].get_longitude());

    BOOST_CHECK_EQUAL(1, after_minute.size());
    BOOST_CHECK_EQUAL(timestamp, after_minute[0].get_timestamp());
    BOOST_CHECK_EQUAL(1, after_minute[0].get_alerts_count());
    BOOST_CHECK_EQUAL(latitude, after_minute[0].get_latitude());
    BOOST_CHECK_EQUAL(longitude, after_minute[0].get_longitude());
}

BOOST_AUTO_TEST_CASE(alert_aggregator_ignores_not_alive) {
    AlertAggregator aggregator;

    aggregator.insert(common_not_alive, smoke);
    aggregator.insert(common_not_alive, ir);

    auto result = aggregator.extractAlerts();

    BOOST_CHECK_EQUAL(0, result.size());
}

BOOST_AUTO_TEST_CASE(alert_aggregator_ignores_below_threshold) {
    AlertAggregator aggregator;

    aggregator.insert(common, smoke_below_threshold);
    aggregator.insert(common, ir_below_threshold);

    auto result = aggregator.extractAlerts();

    BOOST_CHECK_EQUAL(0, result.size());
}