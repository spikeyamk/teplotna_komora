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
}
}