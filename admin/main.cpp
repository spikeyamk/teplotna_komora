#include <QApplication>

#include "main_window.hpp"

int main(int argc, char *argv[]) {
    QApplication app { argc, argv };
    admin::MainWindow main_window {};
    main_window.show();
    app.exec();
}

/*
#include <QApplication>
#include <QStringList>
#include <QDebug>
#include <QWidget>
#include <iostream>
#include <cstdio>
#include <windows.h>

// Constants for AttachConsole

void toggleConsole() {
    if(GetConsoleWindow() == nullptr) {
        constexpr int PARENT_PROCESS = -1;
        if(!AttachConsole(PARENT_PROCESS)) {
            return;
        }
        // Redirect standard streams to the console
        FILE* fp;
        freopen_s(&fp, "CONOUT$", "w", stdout);
        freopen_s(&fp, "CONOUT$", "w", stderr);
        freopen_s(&fp, "CONIN$", "r", stdin);

        std::cout << "Console enabled!" << std::endl;
    }
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QStringList args = app.arguments();
    toggleConsole();
    QWidget window {};
    window.show();

    // Application logic
    qDebug() << "Application started.";
    std::cout << "This is standard output." << std::endl;

    return app.exec();
}
*/