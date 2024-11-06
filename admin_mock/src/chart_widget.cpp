#include <algorithm>

#include <QDateTime>

#include "chart_widget.hpp"

ChartWidget::ChartWidget() :
    layout { new QVBoxLayout(this) },
    front_series { new QLineSeries(this) },
    rear_series { new QLineSeries(this) },
    chart { new QChart() },
    chart_view { new QChartView(chart) },
    timer { new QTimer(this) }
{
    front_series->setName("temp_front");
    rear_series->setName("temp_rear");

    chart->addSeries(front_series);
    chart->addSeries(rear_series);

    chart->createDefaultAxes();
    chart->setTitle("ReadSensors");

    chart_view->setRenderHint(QPainter::Antialiasing);

    layout->addWidget(chart_view);

    connect(timer, &QTimer::timeout, this,
        [this, temp_front = 0.0f, temp_rear = 5.0f ]() mutable {
            push(common::magic::results::ReadSensors { .temp_front = std::sin(temp_front++), .temp_rear = std::sin(temp_rear++) });
        }
    );
    //timer->start(250);
}

ChartWidget::~ChartWidget() {
    delete chart;
}

void ChartWidget::push(const common::magic::results::ReadSensors& read_sensors) { 
    front_series->append(static_cast<float>(front_series->points().size()), read_sensors.temp_front);
    rear_series->append(static_cast<float>(front_series->points().size()), read_sensors.temp_rear);
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