#include <iostream>

#include <QSerialPortInfo>
#include <QJsonDocument>

#include "to_json.hpp"
#include "chart_widget.hpp"
#include "dialog.hpp"

Dialog::Dialog(ChartWidget& chart_widget, QWidget* parent) :
    QDialog { parent },
    layout { new QGridLayout(this) },

    device_label { new QLabel("Device: ") },
    device_combo_box { new QComboBox() },
    transmit_button { new QPushButton("Transmit") },

    command_label { new QLabel("Command: ") },
    command_combo_box { new QComboBox() },

    response_label { new QLabel("Response") },
    response_value { new QLineEdit("N/A") },

    periodic_checkbox { new QCheckBox("Periodic") },
    periodic_timer { new QTimer(this) },
    chart_widget { chart_widget }
{
    for(const auto& e: QSerialPortInfo::availablePorts()) {
        device_combo_box->addItem(e.portName());
    }

    layout->addWidget(device_label, 0, 0);
    layout->addWidget(device_combo_box, 0, 1);
    transmit_button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    layout->addWidget(transmit_button, 0, 2, 3, 1);

    for(const auto& e: command_map.keys()) {
        command_combo_box->addItem(e);
    }

    layout->addWidget(command_label, 1, 0);
    layout->addWidget(command_combo_box, 1, 1);

    layout->addWidget(response_label, 3, 0);
    response_value->setReadOnly(true);
    layout->addWidget(response_value, 3, 1);

    layout->addWidget(periodic_checkbox, 3, 2);

    // Connect the timer to the transmit function for periodic execution
    periodic_timer->setInterval(250);
    connect(periodic_timer, &QTimer::timeout, this, &Dialog::transmit);
    connect(periodic_checkbox, &QCheckBox::checkStateChanged, this, [&](Qt::CheckState state) {
        if(periodic_timer->isActive() && (state == Qt::Unchecked)){
            transmit_button->setEnabled(true);
            periodic_timer->stop();
        }
    });

    connect(transmit_button, &QPushButton::clicked, this, &Dialog::start_transmit);
    connect(&transceiver, &Transceiver::result, this, &Dialog::show_result);
    //connect(&transceiver, &Transceiver::result_buf, this, &Dialog::show_result_buf);
    connect(&transceiver, &Transceiver::error_occured, this, &Dialog::error_occured);
    transceiver.start();
}

void Dialog::start_transmit() {
    transmit();
    if(periodic_checkbox->checkState() == Qt::Checked) {
        transmit_button->setEnabled(false);
        periodic_timer->start();
    }
}

void Dialog::transmit() {
    const QSerialPortInfo device { device_combo_box->currentText() };
    if(device.isNull()) {
        qDebug()
            << "Dialog::transmit():"
            << "device.isNull()";
        return;
    }

    transceiver.transmit(device, command_map[command_combo_box->currentText()]);
}

void Dialog::show_result(const Transceiver::ResultVariant& result) {
    std::visit(
        [&](auto&& result) {
            using Decay = std::decay_t<decltype(result)>;
            if constexpr(std::is_same_v<Decay, common::magic::results::ReadSensors>) {
                static bool was_active_before { false };
                if(periodic_timer->isActive() && (was_active_before == false)) {
                    std::cout << "[\n";
                    was_active_before = true;
                }

                std::cout << QString(QJsonDocument(to_json(result)).toJson(QJsonDocument::Compact)).toStdString() << ",\n";

                chart_widget.push(result);

                if(periodic_timer->isActive() == false) {
                    std::cout << "]\n";
                    was_active_before = false;
                }
            } else if constexpr(std::is_same_v<Decay, common::magic::results::WriteTemp>) {
                qDebug()
                    << "Dialog::show_result():"
                    << "std::is_same_v<Decay, common::magic::results::WriteTemp>: "
                    << "result.value: "
                    << result.value;
            }
        }, result
    );
}

void Dialog::show_result_buf(const QByteArray& result) {
    for(qsizetype i = 0; i < result.size(); i++) {
        std::printf("Dialog::show_result_buf(): result[%lld]: 0x%02X\n", i, static_cast<uint8_t>(result[i]));
    }
}

void Dialog::error_occured(const Transceiver::Error error) {
    qDebug()
        << "Dialog::error_occured():"
        << "error:"
        << error;
}