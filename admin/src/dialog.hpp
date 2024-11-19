#include <QDialog>
#include <QPushButton>
#include <QGridLayout>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QMap>
#include <QCheckBox>
#include <QTimer>
#include <QSlider>
#include <QFileDialog>

#include "transceiver.hpp"
#include "chart_widget.hpp"

class Dialog : public QDialog {
    Q_OBJECT
private:
    QGridLayout* layout { nullptr };

    QLabel* device_label { nullptr };
    QComboBox* device_combo_box { nullptr };

    QPushButton* transmit_button { nullptr };
    QPushButton* save_button { nullptr };

    QLabel* command_label { nullptr };
    const QMap<QString, Transceiver::CommandResult> command_map {{
        { "Nop", { magic::commands::Nop(), magic::results::Nop() } },
        { "ReadTempCtl", { magic::commands::ReadTempCtl(), magic::results::ReadTempCtl() } },
        { "WriteTemp 13.37f", { magic::commands::WriteTemp { 13.37f }, magic::results::WriteTemp() } },
        { "WriteTemp 20.0f", { magic::commands::WriteTemp { 20.0f }, magic::results::WriteTemp() } },
    }};
    QComboBox* command_combo_box { nullptr };

    QLabel* response_label { nullptr };
    QLineEdit* response_value { nullptr };

    QCheckBox* periodic_checkbox { nullptr };
    QTimer* periodic_timer { nullptr };

    static constexpr int PERIODIC_TIMER_SLIDER_MIN { 1 };
    static constexpr int PERIODIC_TIMER_SLIDER_MAX { 1'000 };
    QSlider* periodic_timer_slider { nullptr };
    QLabel* periodic_timer_slider_label { nullptr };

    Transceiver transceiver {};
    ChartWidget& chart_widget;
private slots:
    void start_transmit();
    void transmit();
    void show_result(const Transceiver::ResultVariant& result);
    void show_result_buf(const QByteArray& result);
    void error_occured(const Transceiver::Error error);
    void open_file_picker();
public:
    Dialog(ChartWidget& chart_widget, QWidget* parent = nullptr);
};