#pragma once

#include "sens/sht31/masks.hpp"

namespace sens {
namespace sht31 {
    class Status {
    public:
        Masks::Status::LastWriteDataChecksum::Or last_write_data_checksum;
        Masks::Status::LastCommand::Or last_command;
        Masks::Status::SystemReset::Or system_reset;
        Masks::Status::TrackingAlert::Temp::Or tracking_alert_temp;
        Masks::Status::TrackingAlert::Hum::Or tracking_alert_hum;
        Masks::Status::Heater::Or heater;
        Masks::Status::AlertPending::Or alert_pending;
    public:
        Status() = delete;
        Status(const std::bitset<16>& serialized);
    };
}
}