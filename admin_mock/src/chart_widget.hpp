#pragma once

#include <QWindow>
#include <QLineSeries>
#include <QChart>
#include <QChartView>
#include <QVBoxLayout>
#include <QTimer>

#include "common/magic/results/results.hpp"

class ChartWidget : public QWidget {
    Q_OBJECT
private:
    QVBoxLayout* layout { nullptr };

    QLineSeries* front_series { nullptr };
    QLineSeries* rear_series { nullptr };

    QChart* chart { nullptr };
    QChartView* chart_view { nullptr };
    const qreal msecs_since_epoch;
public:
    ChartWidget();
    ~ChartWidget();
public:
    void push(const common::magic::results::ReadSensors& read_sensors);
private:
    void autoscale_axes();
};