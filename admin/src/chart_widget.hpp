#pragma once

#include <QWindow>
#include <QLineSeries>
#include <QChart>
#include <QChartView>
#include <QVBoxLayout>
#include <QTimer>
#include <QJsonObject>
#include <QJsonArray>

#include "magic/results/results.hpp"

class ChartWidget : public QWidget {
    Q_OBJECT
private:
    QVBoxLayout* layout { nullptr };

    QLineSeries* max31865_front_series { nullptr };
    QLineSeries* max31865_rear_series { nullptr };
    QChart* max31865_chart { nullptr };
    QChartView* max31865_chart_view { nullptr };

    QLineSeries* dmax31865_front_series { nullptr };
    QLineSeries* dmax31865_rear_series { nullptr };
    QChart* dmax31865_chart { nullptr };
    QChartView* dmax31865_chart_view { nullptr };

    QLineSeries* dac_front_series { nullptr };
    QLineSeries* dac_rear_series { nullptr };
    QChart* dac_chart { nullptr };
    QChartView* dac_chart_view { nullptr };

    QLineSeries* sht31_front_series { nullptr };
    QLineSeries* sht31_rear_series { nullptr };
    QChart* sht31_chart { nullptr };
    QChartView* sht31_chart_view { nullptr };

    QLineSeries* p_front_series { nullptr };
    QLineSeries* p_rear_series { nullptr };
    QChart* p_chart { nullptr };
    QChartView* p_chart_view { nullptr };

    QLineSeries* i_front_series { nullptr };
    QLineSeries* i_rear_series { nullptr };
    QChart* i_chart { nullptr };
    QChartView* i_chart_view { nullptr };

    QLineSeries* d_front_series { nullptr };
    QLineSeries* d_rear_series { nullptr };
    QChart* d_chart { nullptr };
    QChartView* d_chart_view { nullptr };

    QJsonArray json_array {};

    const qreal msecs_since_epoch;
public:
    ChartWidget();
    ~ChartWidget();
public:
    void push_to_charts(const magic::results::ReadTempCtl& read_sensors);
    void dump_to_file(const QString& file_path);
private:
    void set_chart_view_scale(QChartView* chart_view);
    void autoscale_axes(QChart* chart, QLineSeries* front_series, QLineSeries* rear_series);
};