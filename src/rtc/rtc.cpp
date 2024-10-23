#include <trielo/trielo.hpp>
#include "rtc.h"
#include "rtc/rtc.hpp"

namespace rtc {
    HAL_StatusTypeDef set_time(const uint8_t hr, const uint8_t min, const uint8_t sec) {
        RTC_TimeTypeDef sTime = {0};
        sTime.Hours = hr;
        sTime.Minutes = min;
        sTime.Seconds = sec;
        sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
        sTime.StoreOperation = RTC_STOREOPERATION_RESET;
        return HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
    }

    HAL_StatusTypeDef set_date(const uint8_t year, const uint8_t month, const uint8_t date, const uint8_t weekday) {
        RTC_DateTypeDef sDate = {0};
        sDate.WeekDay = weekday;
        sDate.Month = month;
        sDate.Date = date;
        sDate.Year = year;
        auto ret { HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) };
        if(ret != HAL_OK) {
            return ret;
        }

        HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR1, 0x2345);  // backup register
        return HAL_OK;
    }

    HAL_StatusTypeDef get_time_date(std::array<char, 100>& time, std::array<char, 100>& date) {
        RTC_DateTypeDef gDate;
        RTC_TimeTypeDef gTime;

        /* Get the RTC current Time */
        auto ret { HAL_RTC_GetTime(&hrtc, &gTime, RTC_FORMAT_BIN) };
        if(ret != HAL_OK) {
            return ret;
        }
        
        /* Get the RTC current Date */
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
        while(1) {
            static std::array<char, 100> time {};
            static std::array<char, 100> date {};
            get_time_date(time, date);
            std::printf("rtc::get_time_test: time: %s date: %s\n", time.data(), date.data());
            HAL_Delay(1'000);
        }
    }
}