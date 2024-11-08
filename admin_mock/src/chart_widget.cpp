#include <algorithm>

#include <QDateTime>

#include "chart_widget.hpp"
#include "chart_widget.hpp"

ChartWidget::ChartWidget() :
    layout { new QVBoxLayout(this) },

    temp_front_series { new QLineSeries(this) },
    temp_rear_series { new QLineSeries(this) },
    temp_chart { new QChart() },
    temp_chart_view { new QChartView(temp_chart) },

    dac_front_series { new QLineSeries(this) },
    dac_rear_series { new QLineSeries(this) },
    dac_chart { new QChart() },
    dac_chart_view { new QChartView(dac_chart) },

    msecs_since_epoch { static_cast<qreal>(QDateTime::currentMSecsSinceEpoch()) }
{
    QPen front_pen(QColor("orange"));
    front_pen.setWidth(2);
    QPen rear_pen(QColorConstants::Green);
    rear_pen.setWidth(2);

    {
        temp_front_series->setName("temp_front");
        temp_front_series->setPen(front_pen);

        temp_rear_series->setName("temp_rear");
        temp_rear_series->setPen(rear_pen);

        temp_chart->addSeries(temp_front_series);
        temp_chart->addSeries(temp_rear_series);

        temp_chart->createDefaultAxes();
        temp_chart->setTitle(typeid(magic::results::ReadSensors()).name());
        temp_chart->axes(Qt::Vertical).front()->setTitleText("Temperature [°C]");
        temp_chart->axes(Qt::Horizontal).front()->setTitleText("Time [s]");

        temp_chart_view->setRenderHint(QPainter::Antialiasing);

        layout->addWidget(temp_chart_view);
    }

    {
        dac_front_series->setName("dac_front");
        dac_front_series->setPen(front_pen);

        dac_rear_series->setName("dac_rear");
        dac_rear_series->setPen(rear_pen);

        dac_chart->addSeries(dac_front_series);
        dac_chart->addSeries(dac_rear_series);

        dac_chart->createDefaultAxes();
        dac_chart->setTitle(typeid(magic::results::ReadSensors()).name());
        dac_chart->axes(Qt::Vertical).front()->setTitleText("DAC Code (max. 4095)");
        dac_chart->axes(Qt::Horizontal).front()->setTitleText("Time [s]");

        dac_chart_view->setRenderHint(QPainter::Antialiasing);

        layout->addWidget(dac_chart_view);
    }
}

ChartWidget::~ChartWidget() {
    delete temp_chart;
    delete dac_chart;
}

void ChartWidget::push(const magic::results::ReadSensors& read_sensors) {
    const qreal current_x_value { (static_cast<qreal>(QDateTime::currentMSecsSinceEpoch()) - msecs_since_epoch) / 1000.0f };
    {
        temp_front_series->append(current_x_value, read_sensors.temp_front);
        temp_rear_series->append(current_x_value, read_sensors.temp_rear);
        autoscale_axes(temp_chart, temp_front_series, temp_rear_series);
    }

    {
        dac_front_series->append(current_x_value, read_sensors.dac_front);
        dac_rear_series->append(current_x_value, read_sensors.dac_rear);
        autoscale_axes(dac_chart, dac_front_series, dac_rear_series);
    }
}

void ChartWidget::autoscale_axes(QChart* chart, QLineSeries* front_series, QLineSeries* rear_series) {
    qreal max_x = -std::numeric_limits<qreal>::infinity();
    qreal min_x = std::numeric_limits<qreal>::infinity();
    qreal max_y = -std::numeric_limits<qreal>::infinity();
    qreal min_y = std::numeric_limits<qreal>::infinity();

    for(const auto& point : front_series->points()) {
        if(point.x() > max_x) {
            max_x = point.x();
        }
        if(point.x() < min_x) {
            min_x = point.x();
        }
    }
    for(const auto& point : rear_series->points()) {
        if(point.x() > max_x) {
            max_x = point.x();
        }
        if(point.x() < min_x) {
            min_x = point.x();
        }
    }

    for(const auto& point : front_series->points()) {
        if(point.y() > max_y) {
            max_y = point.y();
        }
        if(point.y() < min_y) {
            min_y = point.y();
        }
    }
    for(const auto& point : rear_series->points()) {
        if(point.y() > max_y) {
            max_y = point.y();
        }
        if(point.y() < min_y) {
            min_y = point.y();
        }
    }

    chart->axes(Qt::Horizontal).first()->setMax(max_x);
    chart->axes(Qt::Horizontal).first()->setMin(min_x);
    chart->axes(Qt::Vertical).first()->setMax(max_y);
    chart->axes(Qt::Vertical).first()->setMin(min_y);
}