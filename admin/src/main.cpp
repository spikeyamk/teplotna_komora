#include <QApplication>
#include <QScrollArea>

#include "dialog.hpp"
#include "chart_widget.hpp"

int main(int argc, char *argv[]) {
    QApplication app { argc, argv };
    ChartWidget* chart_widget = new ChartWidget();

    QScrollArea *scroll_area = new QScrollArea;
    scroll_area->setWidget(chart_widget);
    scroll_area->setWidgetResizable(true);

    QWidget main_widget;
    QVBoxLayout main_layout(&main_widget);
    main_layout.addWidget(scroll_area);

    main_widget.showMaximized();
    main_widget.show();

    Dialog dialog { *chart_widget };
    dialog.show();
    app.exec();
}