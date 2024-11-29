#pragma once

#include <QWidget>
#include <QGridLayout>
#include <QComboBox>
#include <QPushButton>
#include <QSerialPortInfo>
#include <QThread>
#include <QMutex>
#include <QTimer>

#include "tmp.hpp"
#include "transceiver.hpp"

namespace admin {
    class Control : public QWidget {
        Q_OBJECT
    private:
        QGridLayout* layout;

        const QString serial_port_info_combo_box_empty_text { "QSerialPortInfo::availablePorts().empty()" };
        QComboBox* serial_port_info_combo_box;

        QPushButton* refresh_button;

        const QString CONNECT_DISCONNECT_BUTTON_CONNECT_TEXT;
        const QString CONNECT_DISCONNECT_BUTTON_DISCONNECT_TEXT;
        QPushButton* connect_disconnect_button;

        QPushButton* one_shot_button;
        QTimer* periodic_timer;
        QPushButton* periodic_button;

        QThread* transceiver_thread;
        Transceiver* transceiver;
        bool error_dialog_open;
    public:
        Control(QWidget* parent = nullptr);
        ~Control();
        bool is_connected();
        bool serial_port_info_combo_box_text_contains(const QString& text);
    private:
        Q_SIGNAL void request_connect(const QString port_name);
        Q_SIGNAL void request_disconnect();
        Q_SIGNAL void request_one_shot(const Transceiver::CommandResultPair command_result);
    public:
        Q_SIGNAL void log(const QString message);
        Q_SIGNAL void read_temp_ctl(const magic::results::ReadTempCtl sample);
    private slots:
        void connect_disconnect_button_connect();
        void connect_disconnect_button_disconnect();
        void one_shot();
        void result_arrived(const Transceiver::ResultVariant result_variant);
        void periodic();
        void update_serial_port_info_combo_box_enabled();
        void update_serial_port_info_combo_box_text();
    };
}