#include <trielo/trielo.hpp>

#include "rtc.h"
#include "cmsis_os2.h"
#include "util/rtc.hpp"

namespace util {
namespace rtc {
    HAL_StatusTypeDef set_time(const uint8_t hr, const uint8_t min, const uint8_t sec) {
        RTC_TimeTypeDef sTime {
            .Hours = hr,
            .Minutes = min,
            .Seconds = sec,
            .TimeFormat = RTC_HOURFORMAT12_AM,
            .DayLightSaving = RTC_DAYLIGHTSAVING_NONE,
            .StoreOperation = RTC_CR_BKP,
        };
        return HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
    }

    HAL_StatusTypeDef set_date(const uint8_t year, const uint8_t month, const uint8_t date, const uint8_t weekday) {
        RTC_DateTypeDef sDate {
            .WeekDay = weekday,
            .Month = month,
            .Date = date,
            .Year = year,
        };
        return HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
    }

    HAL_StatusTypeDef get_time_date(std::array<char, 100>& time, std::array<char, 100>& date) {
        RTC_DateTypeDef gDate;
        RTC_TimeTypeDef gTime;

        HAL_StatusTypeDef ret { HAL_RTC_GetTime(&hrtc, &gTime, RTC_FORMAT_BIN) };
        if(ret != HAL_OK) {
            return ret;
        }
        
        ret = HAL_RTC_GetDate(&hrtc, &gDate, RTC_FORMAT_BIN);
        if(ret != HAL_OK) {
            return ret;
        }

        /* Display time Format: hh:mm:ss */
        std::snprintf(time.data(), time.size(), "%02d:%02d:%02d",gTime.Hours, gTime.Minutes, gTime.Seconds);

        /* Display date Format: dd-mm-yyyy */
        std::snprintf(date.data(), date.size(), "%02d-%02d-%2d",gDate.Date, gDate.Month, 2000 + gDate.Year);
        return HAL_OK;
    }

    void get_time_date_test() {
        std::array<char, 100> time {};
        std::array<char, 100> date {};

        while(1) {
            if(Trielo::trielo<get_time_date>(time, date) != HAL_OK) {
                std::printf("rtc::get_time_date_test: Trielo::trielo<get_time_date>(time, date) != HAL_OK\n");
                return;
            }
            std::printf("rtc::get_time_test: time: %s date: %s\n", time.data(), date.data());
            osDelay(1'000);
        }
    }
}
}