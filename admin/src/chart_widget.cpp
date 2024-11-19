#include <algorithm>

#include <QFile>
#include <QDateTime>
#include <QTextStream>
#include <QJsonDocument>

#include "sens/max31865/rtd.hpp"
#include "sens/sht31/temp_hum.hpp"
#include "chart_widget.hpp"
#include "to_json.hpp"

ChartWidget::ChartWidget() :
    layout { new QVBoxLayout(this) },

    max31865_front_series { new QLineSeries(this) },
    max31865_rear_series { new QLineSeries(this) },
    max31865_chart { new QChart() },
    max31865_chart_view { new QChartView(max31865_chart) },

    dac_front_series { new QLineSeries(this) },
    dac_rear_series { new QLineSeries(this) },
    dac_chart { new QChart() },
    dac_chart_view { new QChartView(dac_chart) },

    sht31_inside_series { new QLineSeries(this) },
    sht31_outside_series { new QLineSeries(this) },
    sht31_chart { new QChart() },
    sht31_chart_view { new QChartView(sht31_chart) },

    msecs_since_epoch { static_cast<qreal>(QDateTime::currentMSecsSinceEpoch()) }
{
    QPen front_pen(QColor("orange"));
    front_pen.setWidth(2);
    QPen rear_pen(QColorConstants::Green);
    rear_pen.setWidth(2);

    {
        max31865_front_series->setName("max31865_front");
        max31865_front_series->setPen(front_pen);

        max31865_rear_series->setName("max31865_rear");
        max31865_rear_series->setPen(rear_pen);

        max31865_chart->addSeries(max31865_front_series);
        max31865_chart->addSeries(max31865_rear_series);

        max31865_chart->createDefaultAxes();
        max31865_chart->setTitle(typeid(magic::results::ReadTempCtl()).name());
        max31865_chart->axes(Qt::Vertical).front()->setTitleText("Temperature [°C]");
        max31865_chart->axes(Qt::Horizontal).front()->setTitleText("Time [s]");

        max31865_chart_view->setRenderHint(QPainter::Antialiasing);

        layout->addWidget(max31865_chart_view);
    }

    {
        dac_front_series->setName("dac_front");
        dac_front_series->setPen(front_pen);

        dac_rear_series->setName("dac_rear");
        dac_rear_series->setPen(rear_pen);

        dac_chart->addSeries(dac_front_series);
        dac_chart->addSeries(dac_rear_series);

        dac_chart->createDefaultAxes();
        dac_chart->setTitle(typeid(magic::results::ReadTempCtl()).name());
        dac_chart->axes(Qt::Vertical).front()->setTitleText("DAC Code (max. 4095)");
        dac_chart->axes(Qt::Horizontal).front()->setTitleText("Time [s]");

        dac_chart_view->setRenderHint(QPainter::Antialiasing);

        layout->addWidget(dac_chart_view);
    }

    {
        sht31_inside_series->setName("sht31_inside");
        sht31_inside_series->setPen(front_pen);

        sht31_outside_series->setName("sht31_outside");
        sht31_outside_series->setPen(rear_pen);

        sht31_chart->addSeries(sht31_inside_series);
        sht31_chart->addSeries(sht31_outside_series);

        sht31_chart->createDefaultAxes();
        sht31_chart->setTitle(typeid(magic::results::ReadTempCtl()).name());
        sht31_chart->axes(Qt::Vertical).front()->setTitleText("Temperature [°C]");
        sht31_chart->axes(Qt::Horizontal).front()->setTitleText("Time [s]");

        sht31_chart_view->setRenderHint(QPainter::Antialiasing);

        layout->addWidget(sht31_chart_view);
    }
}

ChartWidget::~ChartWidget() {
    delete max31865_chart;
    delete dac_chart;
    delete sht31_chart;
}

void ChartWidget::push_to_charts(const magic::results::ReadTempCtl& read_sensors) {
    const qreal current_x_value { (static_cast<qreal>(QDateTime::currentMSecsSinceEpoch()) - msecs_since_epoch) / 1000.0f };
    {
        max31865_front_series->append(current_x_value, sens::max31865::RTD(sens::max31865::ADC_Code(read_sensors.max31865_front).serialize()).calculate_approx_temp().value());
        max31865_rear_series->append(current_x_value, sens::max31865::RTD(sens::max31865::ADC_Code(read_sensors.max31865_rear).serialize()).calculate_approx_temp().value());
        autoscale_axes(max31865_chart, max31865_front_series, max31865_rear_series);
    }

    {
        dac_front_series->append(current_x_value, static_cast<qreal>(read_sensors.dac_front));
        dac_rear_series->append(current_x_value, static_cast<qreal>(read_sensors.dac_rear));
        autoscale_axes(dac_chart, dac_front_series, dac_rear_series);
    }

    {
        sht31_inside_series->append(current_x_value, static_cast<qreal>(sens::sht31::TempHum(read_sensors.sht31_inside, 0).calculate_temp()));
        sht31_outside_series->append(current_x_value, static_cast<qreal>(sens::sht31::TempHum(read_sensors.sht31_outside, 0).calculate_temp()));
        autoscale_axes(sht31_chart, sht31_inside_series, sht31_outside_series);
    }

    QJsonObject json_object { to_json(read_sensors) };
    json_object["unix_timestamp"] = current_x_value;
    json_array.append(json_object);
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

void ChartWidget::dump_to_file(const QString& file_path) {
    qDebug()
        << "void ChartWidget::dump_to_file(const QString& file_path)";

    QFile file(file_path);
    if(file.open(QIODevice::WriteOnly)) {
        QJsonDocument json_document(json_array);
        file.write(json_document.toJson(QJsonDocument::Indented)); // Indented for readability
        qDebug() << "Data successfully written to" << file_path;
    } else {
        qDebug() << "Error: Unable to open the file for writing.";
    }
}