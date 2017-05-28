#pragma once

#include <unordered_map>
#include <shared_mutex>
#include <atomic>
#include <vector>
#include "blocks/SensorCommonBlock.h"
#include "blocks/SensorMeasurementBlock.h"
#include "blocks/CentralServerFireAlert.h"

/***
 * This class is thread-safe.
 */
class AlertAggregator {
    using SensorLatitude = uint16_t;
    using SensorLongitude = uint16_t;
    using SensorKey = std::pair<SensorLatitude, SensorLongitude>;

    struct SensorKeyHash {
        std::size_t operator()(const SensorKey &sensor_key) const noexcept {
            return std::hash<SensorLatitude>()(sensor_key.first) ^ std::hash<SensorLongitude>()(sensor_key.second);
        }
    };

    struct SensorSingleData {
        uint16_t latitude;
        uint16_t longitude;

        uint64_t initial_timestamp;
        uint64_t last_timestamp;

        uint32_t alert_count;

        SensorSingleData() = default;
        SensorSingleData(uint16_t latitude, uint16_t longitude, uint64_t initial_timestamp);
        SensorSingleData(const SensorSingleData&) = default;
        SensorSingleData(SensorSingleData&&) = default;

        SensorSingleData& operator =(const SensorSingleData&) = default;
        SensorSingleData& operator =(SensorSingleData&& other);

        void lock();
        void unlock();

        bool should_be_removed() const;

    private:
        std::atomic_flag lock_flag = ATOMIC_FLAG_INIT;
        std::mutex l;
    };

    struct SensorSingleDataLock {
        SensorSingleData& data;

        SensorSingleDataLock(SensorSingleData &data);
        ~SensorSingleDataLock();
    };

    std::unordered_map<
            SensorKey,
            SensorSingleData,
            SensorKeyHash
    > aggregated_data;
    std::shared_mutex aggregated_data_lock;

public:
    void insert(const SensorCommonBlock &common, const SensorMeasurementBlock &measurement);

    std::vector<CentralServerFireAlert> extractAlerts();

private:
    bool should_insert(const SensorMeasurementBlock &block) const;

    void update_data(SensorSingleData &data, const SensorCommonBlock &common);
};
