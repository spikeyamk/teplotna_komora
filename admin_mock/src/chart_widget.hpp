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

    QLineSeries* temp_front_series { nullptr };
    QLineSeries* temp_rear_series { nullptr };
    QChart* temp_chart { nullptr };
    QChartView* temp_chart_view { nullptr };

    QLineSeries* dac_front_series { nullptr };
    QLineSeries* dac_rear_series { nullptr };
    QChart* dac_chart { nullptr };
    QChartView* dac_chart_view { nullptr };

    const qreal msecs_since_epoch;
public:
    ChartWidget();
    ~ChartWidget();
public:
    void push(const common::magic::results::ReadSensors& read_sensors);
private:
    void autoscale_axes(QChart* chart, QLineSeries* front_series, QLineSeries* rear_series);
};