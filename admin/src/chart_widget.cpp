#include <algorithm>

#include <QApplication>
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

    dmax31865_front_series { new QLineSeries(this) },
    dmax31865_rear_series { new QLineSeries(this) },
    dmax31865_chart { new QChart() },
    dmax31865_chart_view { new QChartView(dmax31865_chart) },

    sht31_front_series { new QLineSeries(this) },
    sht31_rear_series { new QLineSeries(this) },
    sht31_chart { new QChart() },
    sht31_chart_view { new QChartView(sht31_chart) },

    p_front_series { new QLineSeries(this) },
    p_rear_series { new QLineSeries(this) },
    p_chart { new QChart() },
    p_chart_view { new QChartView(p_chart) },

    i_front_series { new QLineSeries(this) },
    i_rear_series { new QLineSeries(this) },
    i_chart { new QChart() },
    i_chart_view { new QChartView(i_chart) },

    d_front_series { new QLineSeries(this) },
    d_rear_series { new QLineSeries(this) },
    d_chart { new QChart() },
    d_chart_view { new QChartView(d_chart) },

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
        set_chart_view_scale(max31865_chart_view);
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
        set_chart_view_scale(dac_chart_view);
    }

    {
        p_front_series->setName("p_front");
        p_front_series->setPen(front_pen);

        p_rear_series->setName("p_rear");
        p_rear_series->setPen(rear_pen);

        p_chart->addSeries(p_front_series);
        p_chart->addSeries(p_rear_series);

        p_chart->createDefaultAxes();
        p_chart->setTitle(typeid(magic::results::ReadTempCtl()).name());
        p_chart->axes(Qt::Vertical).front()->setTitleText("DAC Code (max. 4095)");
        p_chart->axes(Qt::Horizontal).front()->setTitleText("Time [s]");

        p_chart_view->setRenderHint(QPainter::Antialiasing);

        layout->addWidget(p_chart_view);
        set_chart_view_scale(p_chart_view);
    }

    {
        i_front_series->setName("i_front");
        i_front_series->setPen(front_pen);

        i_rear_series->setName("i_rear");
        i_rear_series->setPen(rear_pen);

        i_chart->addSeries(i_front_series);
        i_chart->addSeries(i_rear_series);

        i_chart->createDefaultAxes();
        i_chart->setTitle(typeid(magic::results::ReadTempCtl()).name());
        i_chart->axes(Qt::Vertical).front()->setTitleText("DAC Code (max. 4095)");
        i_chart->axes(Qt::Horizontal).front()->setTitleText("Time [s]");

        i_chart_view->setRenderHint(QPainter::Antialiasing);

        layout->addWidget(i_chart_view);
        set_chart_view_scale(i_chart_view);
    }

    {
        d_front_series->setName("d_front");
        d_front_series->setPen(front_pen);

        d_rear_series->setName("d_rear");
        d_rear_series->setPen(rear_pen);

        d_chart->addSeries(d_front_series);
        d_chart->addSeries(d_rear_series);

        d_chart->createDefaultAxes();
        d_chart->setTitle(typeid(magic::results::ReadTempCtl()).name());
        d_chart->axes(Qt::Vertical).front()->setTitleText("DAC Code (max. 4095)");
        d_chart->axes(Qt::Horizontal).front()->setTitleText("Time [s]");

        d_chart_view->setRenderHint(QPainter::Antialiasing);

        layout->addWidget(d_chart_view);
        set_chart_view_scale(d_chart_view);
    }
    
    {
        dmax31865_front_series->setName("dmax31865_front");
        dmax31865_front_series->setPen(front_pen);

        dmax31865_rear_series->setName("dmax31865_rear");
        dmax31865_rear_series->setPen(rear_pen);

        dmax31865_chart->addSeries(dmax31865_front_series);
        dmax31865_chart->addSeries(dmax31865_rear_series);

        dmax31865_chart->createDefaultAxes();
        dmax31865_chart->setTitle(typeid(magic::results::ReadTempCtl()).name());
        dmax31865_chart->axes(Qt::Vertical).front()->setTitleText("dTemperature [°C/s]");
        dmax31865_chart->axes(Qt::Horizontal).front()->setTitleText("Time [s]");

        dmax31865_chart_view->setRenderHint(QPainter::Antialiasing);

        layout->addWidget(dmax31865_chart_view);
        set_chart_view_scale(dmax31865_chart_view);
    }

    {
        sht31_front_series->setName("sht31_front");
        sht31_front_series->setPen(front_pen);

        sht31_rear_series->setName("sht31_rear");
        sht31_rear_series->setPen(rear_pen);

        sht31_chart->addSeries(sht31_front_series);
        sht31_chart->addSeries(sht31_rear_series);

        sht31_chart->createDefaultAxes();
        sht31_chart->setTitle(typeid(magic::results::ReadTempCtl()).name());
        sht31_chart->axes(Qt::Vertical).front()->setTitleText("Temperature [°C]");
        sht31_chart->axes(Qt::Horizontal).front()->setTitleText("Time [s]");

        sht31_chart_view->setRenderHint(QPainter::Antialiasing);

        layout->addWidget(sht31_chart_view);
        set_chart_view_scale(sht31_chart_view);
    }
}

ChartWidget::~ChartWidget() {
    delete max31865_chart;
    delete dac_chart;
    delete dmax31865_chart;
    delete p_chart;
    delete sht31_chart;
    delete i_chart;
    delete d_chart;
}

void ChartWidget::push_to_charts(const magic::results::ReadTempCtl& read_sensors) {
    const qreal current_x_value { (static_cast<qreal>(QDateTime::currentMSecsSinceEpoch()) - msecs_since_epoch) / 1000.0f };

    const qreal max31865_front_value { sens::max31865::RTD(sens::max31865::ADC_Code(read_sensors.max31865_front).serialize()).calculate_approx_temp().value() };
    const qreal max31865_rear_value { sens::max31865::RTD(sens::max31865::ADC_Code(read_sensors.max31865_rear).serialize()).calculate_approx_temp().value() };
    {
        max31865_front_series->append(current_x_value, max31865_front_value);
        max31865_rear_series->append(current_x_value, max31865_rear_value);
        autoscale_axes(max31865_chart, max31865_front_series, max31865_rear_series);
    }

    {
        static qreal max31865_front_before { max31865_front_value };
        static qreal max31865_rear_before { max31865_rear_value };

        dmax31865_front_series->append(current_x_value, max31865_front_before - max31865_front_value);
        dmax31865_rear_series->append(current_x_value, max31865_rear_before - max31865_rear_value);
        autoscale_axes(dmax31865_chart, dmax31865_front_series, dmax31865_rear_series);

        max31865_front_before = max31865_front_value;
        max31865_rear_before = max31865_rear_value;
    }

    {
        dac_front_series->append(current_x_value, static_cast<qreal>(read_sensors.dac_front));
        dac_rear_series->append(current_x_value, static_cast<qreal>(read_sensors.dac_rear));
        autoscale_axes(dac_chart, dac_front_series, dac_rear_series);
    }

    QJsonObject json_object { to_json(read_sensors) };
    json_object["time"] = current_x_value;
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

void ChartWidget::set_chart_view_scale(QChartView* chart_view) {
    chart_view->setMinimumSize(
        QApplication::primaryScreen()->geometry().width() - (QApplication::primaryScreen()->geometry().width() / 12),
        (QApplication::primaryScreen()->geometry().height() / 2) - (QApplication::primaryScreen()->geometry().height() / 12)
    );
}
