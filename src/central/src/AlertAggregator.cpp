#include "AlertAggregator.h"
#include <exception>
#include <sstream>

using namespace std;

namespace {
    const uint64_t ALERT_MERGE_THRESHOLD_IN_SECONDS = 60 * 5;
}

void AlertAggregator::SensorSingleData::lock() {
    while (lock_flag.test_and_set(std::memory_order_acquire));
}

void AlertAggregator::SensorSingleData::unlock() {
    lock_flag.clear(std::memory_order_release);
}

bool AlertAggregator::SensorSingleData::should_be_removed() const {
    return alert_count == 0 &&
            (time(nullptr) - last_timestamp) > ALERT_MERGE_THRESHOLD_IN_SECONDS;
}

AlertAggregator::SensorSingleDataLock::SensorSingleDataLock(AlertAggregator::SensorSingleData &data) : data(data) {
        data.lock();
}

AlertAggregator::SensorSingleDataLock::~SensorSingleDataLock() {
    data.unlock();
}

AlertAggregator::SensorSingleData::SensorSingleData(uint16_t latitude, uint16_t longitude, uint64_t initial_timestamp)
        : latitude(latitude),
          longitude(longitude),
          initial_timestamp(initial_timestamp),
          last_timestamp(initial_timestamp),
          alert_count(1) {}

AlertAggregator::SensorSingleData&
AlertAggregator::SensorSingleData::operator=(AlertAggregator::SensorSingleData &&other) {
    if (&other != this) {
        this->latitude = other.latitude;
        this->longitude = other.longitude;
        this->initial_timestamp = other.initial_timestamp;
        this->last_timestamp = other.last_timestamp;
        this->alert_count = other.alert_count;
    }
    return *this;
}

void AlertAggregator::insert(const SensorCommonBlock &common, const SensorMeasurementBlock &measurement) {
    if (!common.get_is_alive() || !should_insert(measurement))
        return;

    const auto key = SensorKey(common.get_latitude(), common.get_longitude());
    shared_lock<shared_mutex> lockUpdate(aggregated_data_lock);

    auto it = aggregated_data.find(key);
    if (it == aggregated_data.end()) {
        lockUpdate.unlock();
        unique_lock<shared_mutex> lockInsert(aggregated_data_lock);
        aggregated_data[key] = SensorSingleData(common.get_latitude(), common.get_longitude(), common.get_timestamp());
    } else {
        SensorSingleDataLock data_lock(it->second);
        update_data(it->second, common);
    }
}

std::vector<CentralServerFireAlert> AlertAggregator::extractAlerts() {
    unique_lock<shared_mutex> lock(aggregated_data_lock);
    vector<CentralServerFireAlert> result;

    auto it = aggregated_data.begin();
    while (it != aggregated_data.end()) {
        if (it->second.should_be_removed()) {
            it = aggregated_data.erase(it);
            continue;
        }

        if (it->second.alert_count > 0) {
            result.emplace_back(
                    it->second.initial_timestamp,
                    it->second.latitude,
                    it->second.longitude,
                    it->second.alert_count
            );
            it->second.alert_count = 0;
        }

        ++it;
    }

    return move(result);
}

bool AlertAggregator::should_insert(const SensorMeasurementBlock &block) const {
    switch (block.type) {
        case BlockType::smoke_read: {
            return block.getReadValue() > 0.1;
        }

        case BlockType::ir_read: {
            return block.getReadValue() > 0.5;
        }

        case BlockType::temp_read:
            return block.getReadValue() > 100.0;

        default: {
            stringstream str;
            str << "Unsupported measurement block type: " << block.type;
            throw std::runtime_error(str.str());
        }
    }
}

void AlertAggregator::update_data(AlertAggregator::SensorSingleData &data, const SensorCommonBlock &common) {
    assert(data.latitude == common.get_latitude());
    assert(data.longitude == common.get_longitude());

    ++data.alert_count;
    data.last_timestamp = common.get_timestamp();
}
