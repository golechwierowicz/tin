#include <boost/test/unit_test.hpp>
#include "Central.h"
#include "UdpConnection.h"
#include <vector>
#include <chrono>
#include <future>
#include <blocks/BlockReader.h>

using namespace std;

namespace {
    const CentralConfig config = {
            1, // id

            4099, // port

            5000, // heartbeat_interval_ms
            1000, // alert_interval_ms

            "127.0.0.1", // fire_dept_ip
            4001, // fire_dept_port
    };
}

BOOST_AUTO_TEST_CASE(central_config_field_order) {
    BOOST_CHECK_EQUAL(1, config.id);
    BOOST_CHECK_EQUAL(4099, config.port);
    BOOST_CHECK_EQUAL(5000, config.heartbeat_interval_ms);
    BOOST_CHECK_EQUAL(1000, config.alert_interval_ms);
    BOOST_CHECK_EQUAL("127.0.0.1", config.fire_dept_ip);
    BOOST_CHECK_EQUAL(4001, config.fire_dept_port);
}

BOOST_AUTO_TEST_CASE(central_sends_heartbeats_to_fire_dept) {
    const int beat_count = 5;
    vector<chrono::time_point<chrono::system_clock>> heartbeat_points;

    auto fire_dept = thread([&](){
        UdpConnection connection;
        connection.open_socket();
        connection.bind_port(config.fire_dept_port);

        const size_t buffer_size = 512;
        uint8_t buffer[buffer_size];

        for (int i = 0; i < beat_count; ++i) {
            size_t msg_size;
            connection.receive(buffer, buffer_size, msg_size);

            BlockReader reader(buffer, msg_size);

            BOOST_CHECK_EQUAL(1, reader.blocks.size());
            BOOST_CHECK_EQUAL(BlockType::central_server_heartbeat, reader.blocks[0]->type);

            heartbeat_points.push_back(chrono::system_clock::now());
        }
    });

    Central central(config);
    auto dummy = thread([&]() {
        this_thread::sleep_for(chrono::milliseconds(config.heartbeat_interval_ms * (beat_count + 1)));
        central.shutdown();

        // sending dummy message to unlock listen
        UdpConnection connection;
        connection.open_socket();
        connection.bind_port(static_cast<uint16_t>(config.fire_dept_port + 1));
        uint8_t dummy_buffer[] = {0};
        connection.send_msg(dummy_buffer, sizeof(dummy_buffer), connection.get_address("127.0.0.1", config.port));
    });

    central.run();

    fire_dept.join();
    dummy.join();

    for (int i = 1; i < beat_count; ++i) {
        auto diff = heartbeat_points[i] - heartbeat_points[i - 1];
        auto millis = chrono::duration_cast<chrono::milliseconds>(diff).count();

        BOOST_CHECK_LE(millis, config.heartbeat_interval_ms + 200);
    }
}

BOOST_AUTO_TEST_CASE(central_sends_alerts_to_fire_dept) {
    const uint64_t timestamp = static_cast<uint64_t>(time(nullptr));
    const uint16_t latitude = 15;
    const uint16_t longitude = 15;
    
    auto fire_dept = thread([&](){
        UdpConnection connection;
        connection.open_socket();
        connection.bind_port(config.fire_dept_port);

        const size_t buffer_size = 512;
        uint8_t buffer[buffer_size];

        unique_ptr<AbstractBlock> alert_block;
        do {
            size_t msg_size;
            connection.receive(buffer, buffer_size, msg_size);

            for (auto &block : BlockReader(buffer, msg_size).blocks) {
                if (block->type == BlockType::central_server_fire_alert) {
                    alert_block = move(block);
                    break;
                }
            }
        } while (!alert_block);
        
        const CentralServerFireAlert& alert = dynamic_cast<CentralServerFireAlert&>(*alert_block);
        BOOST_CHECK_EQUAL(timestamp, alert.get_timestamp());
        BOOST_CHECK_EQUAL(latitude, alert.get_latitude());
        BOOST_CHECK_EQUAL(longitude, alert.get_longitude());
        BOOST_CHECK_EQUAL(1, alert.get_alerts_count());
    });

    Central central(config);
    auto dummy = thread([&]() {
        // wait for central to fully initialize
        this_thread::sleep_for(chrono::milliseconds(500));

        // setup dummy sensor
        UdpConnection connection;
        connection.open_socket();
        connection.bind_port(static_cast<uint16_t>(config.fire_dept_port + 1));

        // prepare dummy message with alert
        Serializer serializer;
        SensorCommonBlock(timestamp, latitude, longitude, true)
                .serialize(serializer);
        SensorMeasurementBlock(BlockType::smoke_read, 1.0)
                .serialize(serializer);

        uint16_t size;
        auto buffer = serializer.get_buffer(size);

        // send message with alert from sensor
        connection.send_msg(buffer, size, connection.get_address("127.0.0.1", config.port));

        // wait for sending message to fire department
        this_thread::sleep_for(chrono::milliseconds(config.alert_interval_ms * 2));

        central.shutdown();

        // send message to wake up listen thread and shut whole central down
        connection.send_msg(buffer, size, connection.get_address("127.0.0.1", config.port));
    });

    central.run();

    fire_dept.join();
    dummy.join();
}
