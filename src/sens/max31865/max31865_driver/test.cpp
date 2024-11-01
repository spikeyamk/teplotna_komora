#include <trielo/trielo.hpp>

#include "spi.h"
#include "main.h"
#include "sens/max31865/transceiver.hpp"
#include "sens/max31865/extension.hpp"
#include "sens/max31865/test.hpp"

namespace sens {
namespace max31865 {
    int test() {
        max31865::Transceiver transceiver_front { &hspi3, SPI3_TEMP_NSS0_GPIO_Port, SPI3_TEMP_NSS0_Pin };

        if(transceiver_front.init() != HAL_OK) {
            std::printf("sens::max31865::test: transceiver_front.init() != HAL_OK\n");
            return 1;
        }

        const auto read_all_after_init { transceiver_front.read_all() };
        if(read_all_after_init.has_value() == false) {
            std::printf("sens::max31865::test: read_all_after_init.has_value() == false: read_all_after_init.error(): %d\n", read_all_after_init.error());
            return 2;
        }

        if(read_all_after_init.value().front() != std::bitset<8>(0b0000'0000)) {
            std::printf("sens::max31865::test: read_all_after_init.value().front() != std::bitset<8>(0b0000'0000):\n");
            std::printf("sens::max31865::test: read_all_after_init.value().front(): 0x%02lX\n", read_all_after_init.value().front().to_ulong());
            transceiver_front.dump();
            return 3;
        }

        if(transceiver_front.dump() != HAL_OK) {
            std::printf("transceiver_front.dump() != HAL_OK\n");
            return 4;
        }

        if(transceiver_front.write(sens::max31865::RegAddrs::RW::CONFIGURATION, std::bitset<8>(0b0000'0001)) != HAL_OK) {
            std::printf("transceiver_front.write(sens::max31865::RegAddrs::RW::CONFIGURATION, std::bitset<8>(0b0000'0001)) != HAL_OK\n");
            transceiver_front.dump();
            return 5;
        }

        if(transceiver_front.dump() != HAL_OK) {
            std::printf("transceiver_front.dump() != HAL_OK\n");
            return 6;
        }

        if(transceiver_front.read(sens::max31865::RegAddrs::RO::CONFIGURATION) != std::bitset<8>(0b0000'0001)) {
            std::printf("sens::max31865::test: transceiver_front.read(sens::max31865::RegAddrs::RO::CONFIGURATION) != std::bitset<8>(0b0000'0001))\n");
            transceiver_front.dump();
            return 7;
        }

        sens::max31865::Extension extension_front { SPI3_TEMP_NDRDY0_GPIO_Port, SPI3_TEMP_NDRDY0_Pin, transceiver_front };

        if(extension_front.configure(Configuration()) != HAL_OK) {
            std::printf("sens::max31865::test: extension_front.configure(Configuration()) != HAL_OK\n");
            transceiver_front.dump();
            return 8;
        }

        const auto ret_configuration_after_clear_configuration { extension_front.read_configuration() };
        if(ret_configuration_after_clear_configuration.has_value() == false) {
            std::printf("sens::max31865::test: ret_configuration_after_clear_configuration.has_value() == false\n");
            transceiver_front.dump();
            return 9;
        }

        if(ret_configuration_after_clear_configuration.value() != Configuration()) {
            std::printf("sens::max31865::test: ret_configuration_after_clear_configuration.value() != Configuration()\n");
            std::printf("transceiver_front.dump(): \n");
            transceiver_front.dump();
            return 10;
        }

        const Configuration configuration {
            Masks::Configuration::Vbias::Or::ON,
            Masks::Configuration::ConversionMode::Or::AUTO,
            Masks::Configuration::WireMode::Or::TWO_WIRE_OR_FOUR_WIRE
        };

        if(extension_front.configure(configuration) != HAL_OK) {
            std::printf("sens::max31865::test: extension_front.configure(configuration) != HAL_OK\n");
            std::printf("transceiver_front.dump(): \n");
            transceiver_front.dump();
            return 11;
        }
        
        const auto ret_read_configuration { extension_front.read_configuration() };
        if(ret_read_configuration.has_value() == false) {
            std::printf("sens::max31865::test: ret_read_configuration.has_value() == false\n");
            std::printf("transceiver_front.dump(): \n");
            transceiver_front.dump();
            return 12;
        }

        if(ret_read_configuration.value() != configuration) {
            std::printf("sens::max31865::test: ret_read_configuration.value() != configuration\n");
            std::printf("transceiver_front.dump(): \n");
            transceiver_front.dump();
            return 13;
        }

        if(extension_front.set_filter_select(Masks::FilterSelect::Or::FIFTY_HZ) != HAL_OK) {
            std::printf("sens::max31865::test: extension_front.set_filter_select(Masks::FilterSelect::Or::FIFTY_HZ) != HAL_OK\n");
            std::printf("transceiver_front.dump(): \n");
            transceiver_front.dump();
            return 14;
        }
        
        const auto ret_read_filter_select { extension_front.read_filter_select() };
        if(ret_read_filter_select.has_value() == false) {
            std::printf("sens::max31865::test: ret_read_filter_select.has_value() == false\n");
            std::printf("transceiver_front.dump(): \n");
            transceiver_front.dump();
            return 15;
        }

        if(ret_read_filter_select.value() != Masks::FilterSelect::Or::FIFTY_HZ) {
            std::printf("sens::max31865::test: ret_read_filter_select.value() != Masks::FilterSelect::Or::FIFTY_HZ\n");
            std::printf("transceiver_front.dump(): \n");
            transceiver_front.dump();
            return 16;
        }

        const FaultThreshold fault_threshold { RTD(85.0f).adc_code, RTD(-20.0f).adc_code };
        if(extension_front.set_fault_threshold(fault_threshold) != HAL_OK) {
            std::printf("sens::max31865::test: extension_front.set_fault_threshold(fault_threshold) != HAL_OK\n");
            std::printf("transceiver_front.dump(): \n");
            transceiver_front.dump();
            return 17;
        }

        const auto ret_read_fault_threshold { extension_front.read_fault_threshold() };
        if(ret_read_fault_threshold.has_value() == false) {
            std::printf("sens::max31865::test: ret_read_fault_threshold.has_value() == false\n");
            std::printf("transceiver_front.dump(): \n");
            transceiver_front.dump();
            return 18;
        }

        if(ret_read_fault_threshold.value() != fault_threshold) {
            std::printf("sens::max31865::test: ret_read_fault_threshold.value() != fault_threshold\n");
            std::printf("transceiver_front.dump(): \n");
            transceiver_front.dump();
            return 19;
        }

        if(extension_front.configure(Configuration()) != HAL_OK) {
            std::printf("sens::max31865::test: extension_front.configure(Configuration()) != HAL_OK\n");
            std::printf("transceiver_front.dump(): \n");
            transceiver_front.dump();
            return 20;
        }

        if(extension_front.configure(configuration) != HAL_OK) {
            std::printf("sens::max31865::test: extension_front.configure(configuration) != HAL_OK\n");
            std::printf("transceiver_front.dump(): \n");
            transceiver_front.dump();
            return 21;
        }

        for(size_t i = 0; i < 100; i++) {
            if(i == 99) {
                std::printf("sens::max31865::test: if(i == 99): timeout\n");
                std::printf("transceiver_front.dump(): \n");
                transceiver_front.dump();
                return 22;
            }
            if(HAL_GPIO_ReadPin(SPI3_TEMP_NDRDY0_GPIO_Port, SPI3_TEMP_NDRDY0_Pin) == GPIO_PIN_RESET) {
                break;
            }
            HAL_Delay(1);
        }

        const auto read_rtd { extension_front.read_rtd() };
        if(read_rtd.has_value() == false) {
            std::printf("sens::max31865::test: read_rtd.has_value() == false\n");
            std::printf("transceiver_front.dump(): \n");
            transceiver_front.dump();
            return 23;
        }

        if(read_rtd.value().calculate_approx_temp().has_value() == false) {
            std::printf("sens::max31865::test: read_rtd.value().calculate_approx_temp().has_value() == false\n");
            std::printf("transceiver_front.dump(): \n");
            transceiver_front.dump();
            return 24;
        }

        std::printf("read_rtd.value().calculate_approx_temp(): %f\n", read_rtd.value().calculate_approx_temp().value());

        return 0;
    }
}
}