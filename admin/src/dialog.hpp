#include <QDialog>
#include <QPushButton>
#include <QGridLayout>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QMap>
#include <QCheckBox>
#include <QTimer>

#include "transceiver.hpp"

class Dialog : public QDialog {
    Q_OBJECT

private:
    QGridLayout* layout { nullptr };

    QLabel* device_label { nullptr };
    QComboBox* device_combo_box { nullptr };

    QPushButton* transmit_button { nullptr };

    QLabel* command_label { nullptr };
    const QMap<QString, Transceiver::CommandResult> command_map {{
        { "Nop", { common::magic::commands::Nop(), common::magic::results::Nop() } },
        { "ReadSensors", { common::magic::commands::ReadSensors(), common::magic::results::ReadSensors() } },
        { "WriteTemp 13.37f", { common::magic::commands::WriteTemp { 13.37f }, common::magic::results::WriteTemp() } },
        { "WriteTemp 20.0f", { common::magic::commands::WriteTemp { 20.0f }, common::magic::results::WriteTemp() } },
    }};
    QComboBox* command_combo_box { nullptr };

    QLabel* response_label { nullptr };
    QLineEdit* response_value { nullptr };

    QCheckBox* periodic_checkbox { nullptr };
    QTimer* periodic_timer { nullptr };

    Transceiver transceiver {};
private slots:
    void start_transmit();
    void transmit();
    void show_result(const Transceiver::ResultVariant& result);
    void show_result_buf(const QByteArray& result);
    void error_occured(const Transceiver::Error error);
public:
    Dialog(QWidget* parent = nullptr);
};