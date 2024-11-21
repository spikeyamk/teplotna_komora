#include <QApplication>

#include "dialog.hpp"
#include "chart_widget.hpp"

int main(int argc, char *argv[]) {
    ChartWidget chart_widget {};
    chart_widget.resize(400, 400);
    chart_widget.show();

    Dialog dialog { chart_widget };
    dialog.show();
}