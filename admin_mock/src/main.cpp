#include <QApplication>

#include "dialog.hpp"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    Dialog dialog {};
    dialog.show();
    return app.exec();
}