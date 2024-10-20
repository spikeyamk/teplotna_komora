#pragma once

namespace panel {
namespace sevseg {
namespace green_yellow {
    class MAX6549 {
    private:
        static constexpr uint8_t DECODE_MODE { 0x01 };
        static constexpr uint8_t GLOBAL_INTENSITY { 0x02 };
        static constexpr uint8_t SCAN_LIMIT { 0x03 };
        static constexpr uint8_t CONFIGURATION { 0x04 };
        static constexpr uint8_t DIGIT_TYPE { 0x0C };

        static constexpr uint8_t YELLOW_0 { 0x20 };
        static constexpr uint8_t YELLOW_1 { 0x28 };
        static constexpr uint8_t YELLOW_2 { 0x21 };
        static constexpr uint8_t YELLOW_3 { 0x29 };
        static constexpr uint8_t YELLOW_4 { 0x22 };

        static constexpr uint8_t GREEN_0 { 0x24 };
        static constexpr uint8_t GREEN_1 { 0x2C };
        static constexpr uint8_t GREEN_2 { 0x25 };
        static constexpr uint8_t GREEN_3 { 0x2D };
        static constexpr uint8_t GREEN_4 { 0x26 };

        static constexpr std::array<uint8_t, 5> yellow_address_map {
            YELLOW_0,
            YELLOW_1,
            YELLOW_2,
            YELLOW_3,
            YELLOW_4,
        };

        static constexpr std::array<uint8_t, 5> green_address_map {
            GREEN_0,
            GREEN_1,
            GREEN_2,
            GREEN_3,
            GREEN_4,
        };

        static constexpr std::array<uint8_t, 10> digit_address_map {
            YELLOW_0,
            YELLOW_1,
            YELLOW_2,
            YELLOW_3,
            YELLOW_4,
            GREEN_0,
            GREEN_1,
            GREEN_2,
            GREEN_3,
            GREEN_4,
        };
    public:
        MAX6549();

        void select();
        void deselect();

        auto write(const uint8_t address, const uint8_t value);
        uint8_t read(const uint8_t address);
        void write_log(const uint8_t address, const uint8_t value);
        void write_read_log(const uint8_t address, const uint8_t value);
        void read_log(const uint8_t address);

        void clear_all();
        void test_single_segment();
        void test_hex();
        void test_single_segment_single_digit();

        void set_max_global_intensity();

        template<uint8_t level>
        void set_intensity() {
            static_assert(level <= 0x0F, "MAX6549::set_intensity: level must be less than or equal to 0x0F");
            write_log(GLOBAL_INTENSITY, level);
        }

        void normal_operation_disable_blink_global_intensity_clear_all();
        void turn_off_decode_mode();
        void set_max_scan_limit();

        template<uint8_t digits>
        void set_scan_limit() {
            static_assert(digits <= 0x07, "MAX6549::set_scan_limit: digits must be less than or equal to 0x07");
            write_log(SCAN_LIMIT, digits);
        }

        void set_digit_7_to_0_to_7seg_or_16seg_type();
        void shutdown();

        void yellow_show(const float value);
        void green_show(const float value);

        ~MAX6549();
    private:
        void show(const float value, const std::array<uint8_t, 5>& address_map);
    };
    
    void test();
}
}
}