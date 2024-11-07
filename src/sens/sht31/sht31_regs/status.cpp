#include <cstdint>
#include "sens/sht31/status.hpp"

namespace sens {
namespace sht31 {
    Status::Status(const std::bitset<16>& serialized) :
        last_write_data_checksum {
            Masks::Status::LastWriteDataChecksum::Or(
                static_cast<uint16_t>(
                    (
                        Masks::Status::LastWriteDataChecksum::AND
                        & serialized
                    ).to_ulong()
                )
            )
        },
        last_command {
            Masks::Status::LastCommand::Or(
                static_cast<uint16_t>(
                    (
                        Masks::Status::LastCommand::AND
                        & serialized
                    ).to_ulong()
                )
            )
        },
        system_reset {
            Masks::Status::SystemReset::Or(
                static_cast<uint16_t>(
                    (
                        Masks::Status::SystemReset::AND
                        & serialized
                    ).to_ulong()
                )
            )
        },
        tracking_alert_temp {
            Masks::Status::TrackingAlert::Temp::Or(
                static_cast<uint16_t>(
                    (
                        Masks::Status::TrackingAlert::Temp::AND
                        & serialized
                    ).to_ulong()
                )
            )
        },
        tracking_alert_hum {
            Masks::Status::TrackingAlert::Hum::Or(
                static_cast<uint16_t>(
                    (
                        Masks::Status::TrackingAlert::Hum::AND
                        & serialized
                    ).to_ulong()
                )
            )
        },
        heater {
            Masks::Status::Heater::Or(
                static_cast<uint16_t>(
                    (
                        Masks::Status::Heater::AND
                        & serialized
                    ).to_ulong()
                )
            )
        },
        alert_pending {
            Masks::Status::AlertPending::Or(
                static_cast<uint16_t>(
                    (
                        Masks::Status::AlertPending::AND
                        & serialized
                    ).to_ulong()
                )
            )
        }
    {}

    bool Status::operator==(const Status& other) const {
        return last_write_data_checksum == other.last_write_data_checksum &&
               last_command == other.last_command &&
               system_reset == other.system_reset &&
               tracking_alert_temp == other.tracking_alert_temp &&
               tracking_alert_hum == other.tracking_alert_hum &&
               heater == other.heater &&
               alert_pending == other.alert_pending;
    }

    bool Status::operator!=(const Status& other) const {
        return !(*this == other);
    }
}
}

std::ostream& operator<<(std::ostream& os, const sens::sht31::Status& status) {
    os
        << "Status:\n"
        << "  Last Write Data Checksum: 0b" << std::bitset<16>(static_cast<uint16_t>(status.last_write_data_checksum)) << "\n"
        << "  Last Command:             0b" << std::bitset<16>(static_cast<uint16_t>(status.last_command)) << "\n"
        << "  System Reset:             0b" << std::bitset<16>(static_cast<uint16_t>(status.system_reset)) << "\n"
        << "  Tracking Alert (Temp):    0b" << std::bitset<16>(static_cast<uint16_t>(status.tracking_alert_temp)) << "\n"
        << "  Tracking Alert (Hum):     0b" << std::bitset<16>(static_cast<uint16_t>(status.tracking_alert_hum)) << "\n"
        << "  Heater:                   0b" << std::bitset<16>(static_cast<uint16_t>(status.heater)) << "\n"
        << "  Alert Pending:            0b" << std::bitset<16>(static_cast<uint16_t>(status.alert_pending)) << "\n";
    return os;
}