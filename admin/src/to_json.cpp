#include "sens/max31865/rtd.hpp"
#include "sens/sht31/temp_hum.hpp"
#include "magic/results/results.hpp"
#include "to_json.hpp"

namespace admin {
    template<>
    QJsonObject to_json<magic::results::ReadTempCtl>(const magic::results::ReadTempCtl& obj) {
        QJsonObject ret;
        ret["max31865_front"] = sens::max31865::RTD(sens::max31865::ADC_Code(obj.max31865_front).serialize()).calculate_approx_temp().value();
        ret["max31865_rear"] = sens::max31865::RTD(sens::max31865::ADC_Code(obj.max31865_rear).serialize()).calculate_approx_temp().value();
        ret["dac_front"] = obj.dac_front;
        ret["dac_rear"] = obj.dac_rear;
        ret["sht31_inside"] = sens::sht31::TempHum(obj.sht31_inside, 0).calculate_temp();
        ret["sht31_outside"] = sens::sht31::TempHum(obj.sht31_outside, 0).calculate_temp();
        return ret;
    }
}