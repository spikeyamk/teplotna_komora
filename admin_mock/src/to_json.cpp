#include "magic/results/results.hpp"
#include "to_json.hpp"

template<>
QJsonObject to_json<magic::results::ReadSensors>(const magic::results::ReadSensors& obj) {
    QJsonObject ret;
    ret["temp_front"] = obj.temp_front;
    ret["temp_rear"] = obj.temp_rear;
    return ret;
}