#pragma once

#include <QJsonObject>

template<typename T>
QJsonObject to_json(const T& obj);