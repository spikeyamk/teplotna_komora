#include <QCoreApplication>
#include <QTimer>

int main(int argc, char* argv[]) {
    QCoreApplication app(argc, argv);
    qDebug() << "Waiting 5'000 ms for QTimer to expire\n";
    QTimer::singleShot(5'000, &app, &QCoreApplication::quit);
    return app.exec();
}