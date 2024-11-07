#include <algorithm>

#include <QDateTime>

#include "chart_widget.hpp"

ChartWidget::ChartWidget() :
    layout { new QVBoxLayout(this) },
    front_series { new QLineSeries(this) },
    rear_series { new QLineSeries(this) },
    chart { new QChart() },
    chart_view { new QChartView(chart) },
    msecs_since_epoch { static_cast<qreal>(QDateTime::currentMSecsSinceEpoch()) }
{
    front_series->setName("temp_front");
    QPen front_pen(QColor("orange"));
    front_pen.setWidth(2);
    front_series->setPen(front_pen);

    rear_series->setName("temp_rear");
    QPen rear_pen(QColorConstants::Green);
    rear_pen.setWidth(2);
    rear_series->setPen(rear_pen);

    chart->addSeries(front_series);
    chart->addSeries(rear_series);

    chart->createDefaultAxes();
    chart->setTitle(typeid(common::magic::results::ReadSensors()).name());
    chart->axes(Qt::Vertical).front()->setTitleText("Temperature [°C]");
    chart->axes(Qt::Horizontal).front()->setTitleText("Time [s]");

    chart_view->setRenderHint(QPainter::Antialiasing);

    layout->addWidget(chart_view);
}

ChartWidget::~ChartWidget() {
    delete chart;
}

void ChartWidget::push(const common::magic::results::ReadSensors& read_sensors) {
    const qreal current_x_value { (static_cast<qreal>(QDateTime::currentMSecsSinceEpoch()) - msecs_since_epoch) / 1000.0f };
    front_series->append(current_x_value, read_sensors.temp_front);
    rear_series->append(current_x_value, read_sensors.temp_rear);
    autoscale_axes();
}

void ChartWidget::autoscale_axes() {
    qreal max_x = -std::numeric_limits<qreal>::infinity();  // Initialize to very low value
    qreal min_x = std::numeric_limits<qreal>::infinity();   // Initialize to very high value
    qreal max_y = -std::numeric_limits<qreal>::infinity();  // Initialize to very low value
    qreal min_y = std::numeric_limits<qreal>::infinity();   // Initialize to very high value

    // Loop over points in front_series and rear_series for max_x and min_x
    for (const auto& point : front_series->points()) {
        if (point.x() > max_x) {
            max_x = point.x();
        }
        if (point.x() < min_x) {
            min_x = point.x();
        }
    }
    for (const auto& point : rear_series->points()) {
        if (point.x() > max_x) {
            max_x = point.x();
        }
        if (point.x() < min_x) {
            min_x = point.x();
        }
    }

    // Loop over points in front_series and rear_series for max_y and min_y
    for (const auto& point : front_series->points()) {
        if (point.y() > max_y) {
            max_y = point.y();
        }
        if (point.y() < min_y) {
            min_y = point.y();
        }
    }
    for (const auto& point : rear_series->points()) {
        if (point.y() > max_y) {
            max_y = point.y();
        }
        if (point.y() < min_y) {
            min_y = point.y();
        }
    }

    // Now set the chart's axes limits
    chart->axes(Qt::Horizontal).first()->setMax(max_x);
    chart->axes(Qt::Horizontal).first()->setMin(min_x);
    chart->axes(Qt::Vertical).first()->setMax(max_y);
    chart->axes(Qt::Vertical).first()->setMin(min_y);
}