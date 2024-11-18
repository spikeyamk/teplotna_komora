#include "magic/results/results.hpp"
#include "to_json.hpp"

template<>
QJsonObject to_json<magic::results::ReadTempCtl>(const magic::results::ReadTempCtl& obj) {
    QJsonObject ret;
    ret["max31865_front"] = obj.max31865_front;
    ret["max31865_rear"] = obj.max31865_rear;
    return ret;
}