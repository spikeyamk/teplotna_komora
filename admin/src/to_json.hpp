#pragma once

#include <QJsonObject>

namespace admin {
    template<typename T>
    QJsonObject to_json(const T& obj);
}