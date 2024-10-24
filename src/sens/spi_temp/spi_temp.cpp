#include <iostream>
#include <array>
#include <bitset>
#include <expected>

#include "stm32f2xx_hal.h"
#include "cmsis_os2.h"

#include "sens/spi_temp/spi_temp.hpp"
#include "spi.h"
#include "gpio.h"

namespace sens {
namespace spi_temp {
    class MAX31865 {
        struct RegAddrs {
            enum class RW {
                CONFIGURATION             = 0x00,
                HIGH_FAULT_THRESHOLD_MSBS = 0x03,
                HIGH_FAULT_THRESHOLD_LSBS = 0x04,
                LOW_FAULT_THRESHOLD_MSBS  = 0x05,
                LOW_FAULT_THRESHOLD_LSBS  = 0x06,
            };

            enum class RW_RO {
                CONFIGURATION             = 0x00,
                RTD_MSBS                  = 0x01,
                RTD_LSBS                  = 0x02,
                HIGH_FAULT_THRESHOLD_MSBS = 0x03,
                HIGH_FAULT_THRESHOLD_LSBS = 0x04,
                LOW_FAULT_THRESHOLD_MSBS  = 0x05,
                LOW_FAULT_THRESHOLD_LSBS  = 0x06,
                FAULT_STATUS              = 0x07,
            };
        };

        struct Masks {
            using Mask = std::bitset<8>;
            struct Or {
                static constexpr Mask WRITE { 0x80 };
                struct Configuration {
                    struct Vbias {
                        static constexpr Mask OFF { 0b0000'0000 };
                        static constexpr Mask ON  { 0b1000'0000 };
                    };

                    struct ConversionMode {
                        static constexpr Mask OFF  { 0b0000'0000 };
                        static constexpr Mask AUTO { 0b0100'0000 };
                    };

                    struct OneShotAutoClear {
                        static constexpr Mask CLEAR { 0b0010'0000 };
                    };

                    struct WireMode {
                        static constexpr Mask TWO_WIRE_OR_FOUR_WIRE { 0b0000'0000 };
                        static constexpr Mask THREE_WIRE            { 0b0001'0000 };
                    };

                    struct FaultDetection {
                        struct ReadMeaning {
                            static constexpr Mask FAULT_DETECTION_FINISHED                                        { 0b0000'0000 };
                            static constexpr Mask AUTOMATIC_FAULT_DETECTION_STILL_RUNNING                         { 0b0000'0100 };
                            static constexpr Mask MANUAL_CYCLE_ONE_STILL_RUNNING_WAITING_FOR_USER_TO_WRITE_ELEVEN { 0b0000'1000 };
                            static constexpr Mask MANUAL_CYCLE_TWO_STILL_RUNNING                                  { 0b0000'1100 };
                        };

                        struct WriteAction {
                            static constexpr Mask NO_ACTION                                          { 0b0000'0000 };
                            static constexpr Mask FAULT_DETECTION_WITH_AUTOMATIC_DELAY               { 0b0000'0100 };
                            static constexpr Mask RUN_FAULT_DETECTION_WITH_MANUAL_DELAY_CYCLE_ONE    { 0b0000'1000 };
                            static constexpr Mask FINISH_FAULT_DETECTION_WITH_MANUAL_DELAY_CYCLE_TWO { 0b0000'1100 };
                        };
                    };

                    struct FaultStatusAutoClear {
                        static constexpr Mask CLEAR { 0b0000'0010 };
                    };

                    struct FilterSelect {
                        static constexpr Mask SIXTY_HZ { 0b0000'0000 };
                        static constexpr Mask FIFTY_HZ { 0b0000'0001 };
                    };
                };

                struct RTD_LSBs {
                    static constexpr Mask FAULT { 0b0000'0001 };
                    static constexpr Mask DATA  { 0b1111'1110 };
                };

                struct FaultStatus {
                    static constexpr Mask RTD_HIGH_THRESHOLD                                                           { 0b1000'0000 };
                    static constexpr Mask RTD_LOW_THRESHOLD                                                            { 0b0100'0000 };
                    static constexpr Mask REFIN_MINUS_IS_GREATER_THAN_ZERO_POINT_EIGHT_FIVE_TIMES_VBIAS                { 0b0010'0000 };
                    static constexpr Mask REFIN_MINUS_IS_LOWER_THAN_ZERO_POINT_EIGHT_FIVE_TIMES_VBIAS_FORCE_MINUS_OPEN { 0b0001'0000 };
                    static constexpr Mask RTDIN_MINUS_IS_LOWER_THAN_ZERO_POINT_EIGHT_FIVE_TIMES_VBIAS_FORCE_MINUS_OPEN { 0b0000'1000 };
                    static constexpr Mask OVERVOLTAGE_UNDERVOLTAGE_FAULT                                               { 0b0000'0100 };
                };
            };

            struct And {
                static constexpr Mask READ  { 0x7F };

                struct Configuration {
                    static constexpr Mask VBIAS                   { 0b1000'0000 };
                    static constexpr Mask CONVERSION_MODE         { 0b0100'0000 };
                    static constexpr Mask ONE_SHOT_AUTO_CLEAR     { 0b0010'0000 };
                    static constexpr Mask WIRE_MODE               { 0b0001'0000 };
                    static constexpr Mask FAULT_DETECTION         { 0b0000'1100 };
                    static constexpr Mask FAULT_STATUS_AUTO_CLEAR { 0b0000'0010 };
                    static constexpr Mask FILTER_SELECT           { 0b0000'0001 };
                };

                struct RTD_LSBs {
                    static constexpr Mask FAULT { 0b0000'0001 };
                    static constexpr Mask DATA  { 0b1111'1110 };
                };

                struct FaultStatus {
                    static constexpr Mask RTD_HIGH_THRESHOLD                                                           { 0b1000'0000 };
                    static constexpr Mask RTD_LOW_THRESHOLD                                                            { 0b0100'0000 };
                    static constexpr Mask REFIN_MINUS_IS_GREATER_THAN_ZERO_POINT_EIGHT_FIVE_TIMES_VBIAS                { 0b0010'0000 };
                    static constexpr Mask REFIN_MINUS_IS_LOWER_THAN_ZERO_POINT_EIGHT_FIVE_TIMES_VBIAS_FORCE_MINUS_OPEN { 0b0001'0000 };
                    static constexpr Mask RTDIN_MINUS_IS_LOWER_THAN_ZERO_POINT_EIGHT_FIVE_TIMES_VBIAS_FORCE_MINUS_OPEN { 0b0000'1000 };
                    static constexpr Mask OVERVOLTAGE_UNDERVOLTAGE_FAULT                                               { 0b0000'0100 };
                };
            };
        };
    private:
        GPIO_TypeDef* nss_port;
        const uint16_t nss_pin;
    public:
        MAX31865(GPIO_TypeDef* nss_port, const uint16_t nss_pin) :
            nss_port { nss_port },
            nss_pin  { nss_pin }
        {}

        void select() const {
            HAL_GPIO_WritePin(nss_port, nss_pin, GPIO_PIN_RESET);
        }

        void deselect() const {
            HAL_GPIO_WritePin(nss_port, nss_pin, GPIO_PIN_SET);
        }

        HAL_StatusTypeDef init() const {
            select();
            osDelay(10);
            const std::array<uint8_t, 2> init_configuration {
                static_cast<uint8_t>(RegAddrs::RW::CONFIGURATION),
                static_cast<uint8_t>(Masks::Or::Configuration::FaultStatusAutoClear::CLEAR.to_ulong()),
            };
            const HAL_StatusTypeDef ret { HAL_SPI_Transmit(
                &hspi3,
                init_configuration.data(),
                init_configuration.size(),
                500
            ) };
            deselect();
            osDelay(100);
            return ret;
        }

        HAL_StatusTypeDef write(const RegAddrs::RW address, const std::bitset<8>& value) const {
            select();
            const std::array<uint8_t, 2> buf {
                static_cast<uint8_t>((std::bitset<8>(static_cast<uint8_t>(address)) | Masks::Or::WRITE).to_ulong()),
                static_cast<uint8_t>(value.to_ulong())
            };
            const HAL_StatusTypeDef ret { HAL_SPI_Transmit(&hspi3, buf.data(), buf.size(), 500) };
            deselect();
            return ret;
        }
        
        std::expected<uint8_t, HAL_StatusTypeDef> read(const RegAddrs::RW_RO address) const {
            select();

            const uint8_t buf { static_cast<uint8_t>((std::bitset<8>(static_cast<uint8_t>(address)) & Masks::And::READ).to_ulong())};
            HAL_StatusTypeDef ret_err { HAL_SPI_Transmit(&hspi3, &buf, sizeof(buf), 500) };
            if(ret_err != HAL_OK) {
                deselect();
                return std::unexpected(ret_err);
            }

            uint8_t ret { 0x00 };
            ret_err = HAL_SPI_Receive(&hspi3, &ret, sizeof(ret), 500);
            deselect();

            if(ret_err != HAL_OK) {
                return std::unexpected(ret_err);
            }

            return ret;
        }

        std::expected<std::array<uint8_t, 8>, HAL_StatusTypeDef> read_all() const {
            select();

            const uint8_t buf { static_cast<uint8_t>(RegAddrs::RW_RO::CONFIGURATION) & static_cast<uint8_t>(Masks::And::READ.to_ulong()) };
            HAL_SPI_Transmit(&hspi3, &buf, sizeof(buf), 500);
            std::array<uint8_t, 8> ret_regs {};
            for(uint8_t& e: ret_regs) {
                const HAL_StatusTypeDef ret_err { HAL_SPI_Receive(&hspi3, &e, sizeof(e), 500) };
                if(ret_err != HAL_OK) {
                    deselect();
                    return std::unexpected(ret_err);
                }
            }

            deselect();
            return ret_regs;
        }

        std::expected<HAL_StatusTypeDef, bool> set_vbias(const bool state) {
            const auto before { read(RegAddrs::RW_RO::CONFIGURATION) };
            if(before.has_value() == false) {
                return std::unexpected(before.error());
            }

            /*
            if(state) {

            }
            */
            return HAL_OK;
        }
    };
}
}

namespace sens {
namespace spi_temp {
    namespace select {
        void a() {
            HAL_GPIO_WritePin(SPI3_TEMP_NSS0_GPIO_Port, SPI3_TEMP_NSS0_Pin, GPIO_PIN_RESET);
        }

        void b() {
            HAL_GPIO_WritePin(SPI3_TEMP_NSS1_GPIO_Port, SPI3_TEMP_NSS1_Pin, GPIO_PIN_RESET);
        }
    }

    namespace deselect {
        void a() {
            HAL_GPIO_WritePin(SPI3_TEMP_NSS0_GPIO_Port, SPI3_TEMP_NSS0_Pin, GPIO_PIN_SET);
        }

        void b() {
            HAL_GPIO_WritePin(SPI3_TEMP_NSS1_GPIO_Port, SPI3_TEMP_NSS1_Pin, GPIO_PIN_SET);
        }
    }

    namespace write {
        auto a(const uint8_t address, const uint8_t value) {
            select::a();
            //HAL_Delay(10);
            const std::array<uint8_t, 2> buf { (address | 0x80), value };
            const auto ret { HAL_SPI_Transmit(&hspi3, buf.data(), buf.size(), 500) };
            deselect::a();
            return ret;
        }

        auto b(const uint8_t address, const uint8_t value) {
            select::b();
            //HAL_Delay(10);
            const std::array<uint8_t, 2> buf { (address | 0x80), value };
            const auto ret { HAL_SPI_Transmit(&hspi3, buf.data(), buf.size(), 500) };
            deselect::b();
            return ret;
        }
    }

    namespace read {
        uint8_t a(const uint8_t address) {
            select::a();
            //HAL_Delay(10);
            const uint8_t buf { (address & 0x7F) };
            HAL_SPI_Transmit(&hspi3, &buf, sizeof(buf), 500);
            uint8_t ret { 0x00 };
            HAL_SPI_Receive(&hspi3, &ret, sizeof(ret), 500);
            deselect::a();
            return ret;
        }

        uint8_t b(const uint8_t address) {
            select::b();
            //HAL_Delay(10);
            const uint8_t buf { (address & 0x7F) };
            HAL_SPI_Transmit(&hspi3, &buf, sizeof(buf), 500);
            uint8_t ret { 0x00 };
            HAL_SPI_Receive(&hspi3, &ret, sizeof(ret), 500);
            deselect::b();
            return ret;
        }
    }

    namespace init {
        static const std::array<uint8_t, 2> defaults_config_reset_fault { 0x00, 0xC2 };
        void a() {
            select::a();
            HAL_Delay(10);
            HAL_SPI_Transmit(&hspi3, defaults_config_reset_fault.data(), defaults_config_reset_fault.size(), 500);
            deselect::a();
        }

        void b() {
            select::b();
            HAL_Delay(10);
            HAL_SPI_Transmit(&hspi3, defaults_config_reset_fault.data(), defaults_config_reset_fault.size(), 500);
            deselect::b();
        }
    }

    namespace dump {
        void a() {
            select::a();

            const uint8_t buf { 0x00 };
            HAL_SPI_Transmit(&hspi3, &buf, sizeof(buf), 500);
            std::array<uint8_t, 8> ret_regs {};
            for(auto& e: ret_regs) {
                HAL_SPI_Receive(&hspi3, &e, sizeof(e), 500);
            }
            for(uint8_t i = 0; i < ret_regs.size(); i++) {
                std::printf("max31865::dump::a: ret_regs[%u]: 0x%02X\n", i, ret_regs[i]);
            }

            deselect::a();
        }

        void b() {
            select::b();

            const uint8_t buf { 0x00 };
            HAL_SPI_Transmit(&hspi3, &buf, sizeof(buf), 500);
            std::array<uint8_t, 8> ret_regs {};
            for(auto& e: ret_regs) {
                HAL_SPI_Receive(&hspi3, &e, sizeof(e), 500);
            }
            for(uint8_t i = 0; i < ret_regs.size(); i++) {
                std::printf("max31865::dump::b: ret_regs[%u]: 0x%02X\n", i, ret_regs[i]);
            }

            deselect::b();
        }
    }

    void test() {
        init::a();
        HAL_Delay(1000);
        init::b();
        HAL_Delay(1000);

        dump::a();
        dump::b();
        // For sensor A
        std::printf(
            "read::a(0x00): 0x%02X\n", 
            read::a(0x00)
        );

        std::printf(
            "write::a(0x00, 0xC2): 0x%02X\n", 
            write::a(0x00, 0xC2)
        );
        std::printf(
            "read::a(0x00): 0x%02X\n", 
            read::a(0x00)
        );

        std::printf(
            "write::a(0x00, 0x40): 0x%02X\n", 
            write::a(0x00, 0x40)
        );
        std::printf(
            "read::a(0x00): 0x%02X\n", 
            read::a(0x00)
        );


        // For sensor B
        std::printf(
            "read::b(0x00): 0x%02X\n", 
            read::b(0x00)
        );

        std::printf(
            "write::b(0x00, 0xC2): 0x%02X\n", 
            write::b(0x00, 0xC2)
        );
        std::printf(
            "read::b(0x00): 0x%02X\n", 
            read::b(0x00)
        );

        std::printf(
            "write::b(0x00, 0x40): 0x%02X\n", 
            write::b(0x00, 0x40)
        );
        std::printf(
            "read::b(0x00): 0x%02X\n", 
            read::b(0x00)
        );
    }
}
}