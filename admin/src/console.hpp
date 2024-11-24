#pragma once

#include <functional>

#include <QWidget>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QLineEdit>
#include <QMap>
#include <QDateTime>

namespace admin {
    class Console : public QWidget {
        Q_OBJECT
    private:
        QVBoxLayout* layout;
        QTextEdit* output;
        QLineEdit* input;
        const QMap<QString, std::function<void(Console&)>> command_map;
    public:
        Console(QWidget* parent = nullptr);
    public:
        Q_SLOT void log(const QString message);
        Q_SIGNAL void request_exit();
    private:
        void answer(const QString& message);
    private:
        static void help(Console& self);
        static void exit(Console& self);
        static void clear(Console& self);
        static void placeholder(Console& self);
    };
}