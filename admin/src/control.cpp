#include <ranges>

#include <QMessageBox>

#include "control.hpp"

namespace admin {
    Control::Control(QWidget* parent) :
        QWidget { parent },
        layout { new QGridLayout(this) },

        serial_port_info_combo_box_empty_text { "QSerialPortInfo::availablePorts().empty()" },
        serial_port_info_combo_box { new QComboBox() },

        refresh_button { new QPushButton("Refresh") },

        CONNECT_DISCONNECT_BUTTON_CONNECT_TEXT { "Connect" },
        CONNECT_DISCONNECT_BUTTON_DISCONNECT_TEXT { "Disconnect" },
        connect_disconnect_button { new QPushButton(CONNECT_DISCONNECT_BUTTON_CONNECT_TEXT) },

        one_shot_button { new QPushButton("One Shot") },
        periodic_button { new QPushButton("Periodic") },
        save_button { new QPushButton("Save") }, 
        transceiver_thread { new QThread(this) },
        transceiver { new Transceiver() },
        error_dialog_open { false }
    {
        layout->addWidget(serial_port_info_combo_box, 0, 0);
        serial_port_info_combo_box->setEnabled(false);
        connect(serial_port_info_combo_box, &QComboBox::currentIndexChanged, this, &Control::update_serial_port_info_combo_box_enabled);

        layout->addWidget(refresh_button, 0, 1);
        connect(refresh_button, &QPushButton::clicked, this, &Control::update_serial_port_info_combo_box_text);

        layout->addWidget(connect_disconnect_button, 0, 2);
        connect_disconnect_button->setEnabled(false);
        connect(connect_disconnect_button, &QPushButton::clicked, this, &Control::connect_disconnect_button_connect);

        layout->addWidget(one_shot_button);
        one_shot_button->setEnabled(false);
        connect(one_shot_button, &QPushButton::clicked, this, &Control::one_shot);

        layout->addWidget(periodic_button);
        periodic_button->setEnabled(false);
        connect(periodic_button, &QPushButton::clicked, this, &Control::periodic);

        layout->addWidget(save_button);
        save_button->setEnabled(false);

        transceiver->moveToThread(transceiver_thread);
        connect(transceiver_thread, &QThread::finished, transceiver, &QObject::deleteLater);

        connect(transceiver, &Transceiver::error_occured, this,
            [this](const Transceiver::Error error, const QString message) {
                const QString tmp_message {
                    QString(typeid(error).name())
                    + ": &Transceiver::error_occured: "
                    + QVariant::fromValue(error).toString()
                    + (message.isEmpty() ? message : (": " + message))
                };

                if(error_dialog_open == false) {
                    error_dialog_open = true;
                    QMessageBox* error_message_box {
                        new QMessageBox(
                            QMessageBox::Critical,
                            "Error",
                            tmp_message,
                            QMessageBox::Ok,
                            this
                        )
                    };
                    connect(error_message_box, &QMessageBox::finished, this,
                        [this]() { error_dialog_open = false; }
                    );
                    error_message_box->show();
                }

                emit log(QString(typeid(*this).name()) + ": " + tmp_message);
                connect_disconnect_button->setEnabled(true);
            }
        );

        connect(transceiver, &Transceiver::connected_changed, this, [this](const bool connected) {
            serial_port_info_combo_box->setEnabled(!connected);
            refresh_button->setEnabled(!connected);

            connect_disconnect_button->setEnabled(true);
            if(connected) {
                connect_disconnect_button->setText(CONNECT_DISCONNECT_BUTTON_DISCONNECT_TEXT);
                disconnect(connect_disconnect_button, &QPushButton::clicked, nullptr, nullptr);
                connect(connect_disconnect_button, &QPushButton::clicked, this, &Control::connect_disconnect_button_disconnect);
            } else {
                connect_disconnect_button->setText(CONNECT_DISCONNECT_BUTTON_CONNECT_TEXT);
                disconnect(connect_disconnect_button, &QPushButton::clicked, nullptr, nullptr);
                connect(connect_disconnect_button, &QPushButton::clicked, this, &Control::connect_disconnect_button_connect);
            }

            one_shot_button->setEnabled(connected);
            periodic_button->setEnabled(connected);

            emit log(
                QString(typeid(*this).name())
                + ": &Transceiver::connected_changed: connected: "
                + QVariant(connected).toString()
            );
        });

        connect(this, &Control::request_connect, transceiver, &Transceiver::connect);
        connect(this, &Control::request_disconnect, transceiver, &Transceiver::disconnect);
        connect(this, &Control::request_one_shot, transceiver, &Transceiver::one_shot);
        connect(transceiver, &Transceiver::result_arrived, this, &Control::result_arrived);

        transceiver_thread->start();
    }

    Control::~Control() {
        transceiver_thread->quit();
        transceiver_thread->wait();
    }

    bool Control::is_connected() {
        return transceiver->is_connected();
    }

    bool Control::serial_port_info_combo_box_text_contains(const QString& text) {
        return serial_port_info_combo_box->findText(text) != -1;
    }

    void Control::connect_disconnect_button_connect() {
        connect_disconnect_button->setEnabled(false);
        const QString selected_serial_port_info_port_name { serial_port_info_combo_box->currentText().split(' ').first() };
        emit request_connect(selected_serial_port_info_port_name);
    }

    void Control::connect_disconnect_button_disconnect() {
        emit request_disconnect();
    }

    void Control::one_shot() {
        one_shot_button->setEnabled(false);
        periodic_button->setEnabled(false);
        emit request_one_shot(Transceiver::CommandResultPair(magic::commands::ReadTempCtl(), magic::results::ReadTempCtl()));
    }

    void Control::result_arrived(const Transceiver::ResultVariant result_variant) {
        std::visit(overloaded {
            [this](const magic::results::ReadTempCtl& result) {
                emit read_temp_ctl(result);
            },
            [](const auto&) {}
        }, result_variant);
    }

    void Control::periodic() {
        one_shot_button->setEnabled(false);
        periodic_button->setEnabled(false);
    }

    void Control::update_serial_port_info_combo_box_enabled() {
        if(serial_port_info_combo_box->findText(serial_port_info_combo_box_empty_text) != -1) {
            serial_port_info_combo_box->setEnabled(false);
            connect_disconnect_button->setEnabled(false);
            return;
        }

        const bool enabled { serial_port_info_combo_box->count() > 0 };
        serial_port_info_combo_box->setEnabled(enabled);
        connect_disconnect_button->setEnabled(enabled);
    }

    void Control::update_serial_port_info_combo_box_text() {
        if(QSerialPortInfo::availablePorts().empty()) {
            serial_port_info_combo_box->clear();
            serial_port_info_combo_box->addItem(serial_port_info_combo_box_empty_text);
            return;
        }

        const QStringList available_ports_text_items {
            std::ranges::transform_view(
                QSerialPortInfo::availablePorts(),
                [](const QSerialPortInfo& e) {
                    return e.portName()
                        + " " + e.systemLocation()
                        + " " + e.description()
                        + " " + e.manufacturer()
                        + " " + e.serialNumber()
                        + " " + (e.hasVendorIdentifier() 
                            ? QByteArray::number(e.vendorIdentifier(), 16)
                            : QByteArray())
                        + " " + (e.hasProductIdentifier() 
                            ? QByteArray::number(e.productIdentifier(), 16)
                            : QByteArray());
                }
            ) | std::ranges::to<QStringList>()
        };

        std::ranges::for_each(
            std::ranges::views::filter(
                available_ports_text_items,
                [this](const QString& e) {
                    return serial_port_info_combo_box_text_contains(e) == false;
                }
            ),
            [this](const QString& e) {
                serial_port_info_combo_box->addItem(e);
            }
        );

        std::ranges::for_each(
            std::ranges::views::iota(0, serial_port_info_combo_box->count())
            | std::ranges::views::filter(
                [&](const int e) {
                    return available_ports_text_items.contains(serial_port_info_combo_box->itemText(e)) == false;
                }
            ),
            [this](const int e) {
                serial_port_info_combo_box->removeItem(e);
            }
        );
    }
}