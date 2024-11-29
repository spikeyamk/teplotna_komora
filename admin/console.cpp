#include <QDateTime>

#include "console.hpp"

namespace admin {
    Console::Console(QWidget* parent) :
        QWidget { parent },
        layout { new QVBoxLayout(this) },
        output { new QTextEdit(this) },
        input { new QLineEdit(this) },
        command_map {
            { "help", help },
            { "exit", exit },
            { "clear", clear },
            { "placeholder", placeholder },
        }
    {
        output->setReadOnly(true);
        layout->addWidget(output);
        layout->addWidget(input);
        connect(input, &QLineEdit::returnPressed, this, [this]() {
            const auto find_it { command_map.find(input->text()) };
            if(find_it == command_map.end()) {
                answer("ERROR: Uknown command: '" + input->text() + "' try: 'help' to list available commands.");
            } else {
                find_it.value()(*this);
            }
            input->clear();
        });
    }

    void Console::log(const QString message) {
        output->append(QString("[%1] %2").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz"),  message));
    }

    void Console::answer(const QString& message) {
        output->append(message);
    }

    void Console::help(Console& self) {
        for(const auto& e: self.command_map.keys()) {
            self.answer(e);
        }
    }

    void Console::exit(Console& self) {
        emit self.request_exit();
    }

    void Console::clear(Console& self) {
        self.output->clear();
    }

    void Console::placeholder(Console& self) {
        self.answer("Console::placeholder");
    }
}