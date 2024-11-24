#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QLineSeries>
#include <QChart>
#include <QChartView>
#include <QJsonArray>

#include "magic/magic.hpp"

namespace admin {
    class Charts : public QWidget {
        Q_OBJECT
    private:
        QVBoxLayout* layout;
        QWidget* scroll_content;
        QVBoxLayout* scroll_content_layout;
        QScrollArea* scroll_area;

        QLineSeries* max31865_front_series;
        QLineSeries* max31865_rear_series;
        QChart* max31865_chart;
        QChartView* max31865_chart_view;

        QLineSeries* dmax31865_front_series;
        QLineSeries* dmax31865_rear_series;
        QChart* dmax31865_chart;
        QChartView* dmax31865_chart_view;

        QLineSeries* dac_front_series;
        QLineSeries* dac_rear_series;
        QChart* dac_chart;
        QChartView* dac_chart_view;

        QLineSeries* p_front_series;
        QLineSeries* p_rear_series;
        QChart* p_chart;
        QChartView* p_chart_view;

        QLineSeries* i_front_series;
        QLineSeries* i_rear_series;
        QChart* i_chart;
        QChartView* i_chart_view;

        QLineSeries* d_front_series;
        QLineSeries* d_rear_series;
        QChart* d_chart;
        QChartView* d_chart_view;

        QLineSeries* sht31_front_series;
        QLineSeries* sht31_rear_series;
        QChart* sht31_chart;
        QChartView* sht31_chart_view;

        QJsonArray* json_array;
    public:
        Charts(QWidget* parent = nullptr);
        ~Charts();
    private:
        void setup_chart(
            QLineSeries*& first_series,
            QLineSeries*& second_series,
            QChart*& chart,
            QChartView*& chart_view,
            const QString& first_series_name,
            const QString& second_series_name,
            const QString& chart_title,
            const QString& x_axis_text,
            const QString& y_axis_text
        );
        void autoscale_axes(QChart* chart, QLineSeries* first_series, QLineSeries* second_series);
        void push_to_chart(
            QLineSeries* first_series,
            QLineSeries* second_series,
            QChart* chart,
            const qreal x_value,
            const qreal first_y_value,
            const qreal second_y_value
        );
    public:
        Q_SLOT void push_to_charts(const magic::results::ReadTempCtl sample);
        void dump_to_file(const QString& file_path);
        Q_SIGNAL void log(const QString error);
    };
}