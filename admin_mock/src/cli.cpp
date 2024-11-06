#include <expected>
#include <QCoreApplication>
#include <QDebug>
#include <QFileInfo>
#include <QSerialPortInfo>
#include <QSerialPort>
#include <QThread>

#include "common/magic/magic.hpp"

bool has_option(const QStringList& arguments, const QString& option) {
    for(const QString& e: arguments) {
        if(e == option) {
            return true;
        }
    }

    return false;
}

const float MIN_TEMP { -40.0f };
const float MAX_TEMP { 85.0f };

void print_help(const QString& executable_name) {
    qDebug().noquote().nospace()
        << "Usage: " << executable_name << " [OPTION]...\n"
        << "  --device=[NAME]         device name\n"
        << "  --help                  display help\n"
        << "  --no-gui                run without graphical interface\n"
        << "  --read=[SAMPLES]        read sensors\n"
        << "  --write=[TEMPERATURE]   write temperature between " << MIN_TEMP << " and " << MAX_TEMP;
}

void print_error(const QString& executable_name) {
    qDebug().noquote().nospace()
        << executable_name
        << ": error: unable to parse arguments, try '--help' for more help";
}

template<typename T>
std::optional<T> extract_from_option_with_prefix(const QStringList& arguments, const QString& option_prefix);

template<>
std::optional<QString> extract_from_option_with_prefix(const QStringList& arguments, const QString& option_prefix) {
    for(size_t i = 0; i < arguments.size(); i++) {
        if((arguments[i].size() > option_prefix.size()) && arguments[i].startsWith(option_prefix)) {
            return arguments[i].mid(option_prefix.size());
        }
    }

    return std::nullopt;
}

template<typename T, typename Parser>
std::optional<T> extract_from_option_with_prefix(const QStringList& arguments, const QString& option_prefix) {
    const std::optional<QString> extracted_value { extract_from_option_with_prefix<QString>(arguments, option_prefix) };
    if(extracted_value.has_value() == false) {
        return std::nullopt;
    }

    bool parse_success { false };
    const T ret { Parser::run(extracted_value.value(), parse_success) };
    if(parse_success == false) {
        return std::nullopt;
    }

    return ret;
}

template<>
std::optional<QSerialPortInfo> extract_from_option_with_prefix<QSerialPortInfo>(const QStringList& arguments, const QString& option_prefix) {
    class Parser {
    public:
        static QSerialPortInfo run(const QString& string, bool& success) {
            const QSerialPortInfo ret { QFileInfo(string).fileName() };
            if(ret.isNull()) {
                success = false;
                return ret;
            }

            success = true;
            return ret;
        }
    };

    return extract_from_option_with_prefix<QSerialPortInfo, Parser>(arguments, option_prefix);
}

template<>
std::optional<float> extract_from_option_with_prefix<float>(const QStringList& arguments, const QString& option_prefix) {
    class Parser {
    public:
        static float run(const QString& string, bool& success) {
            return string.toFloat(&success);
        }
    };

    return extract_from_option_with_prefix<float, Parser>(arguments, option_prefix);
}

template<>
std::optional<quint64> extract_from_option_with_prefix<quint64>(const QStringList& arguments, const QString& option_prefix) {
    class Parser {
    public:
        static quint64 run(const QString& string, bool& success) {
            return string.toULongLong(&success);
        }
    };

    return extract_from_option_with_prefix<quint64, Parser>(arguments, option_prefix);
}

std::optional<QSerialPortInfo> extract_device(const QStringList& arguments) {
    return extract_from_option_with_prefix<QSerialPortInfo>(arguments, "--device=");
}

std::optional<quint64> extract_read(const QStringList& arguments) {
    return extract_from_option_with_prefix<quint64>(arguments, "--read=");
}

std::optional<float> extract_write(const QStringList& arguments) {
    const std::optional<float> ret { extract_from_option_with_prefix<float>(arguments, "--write=") };

    if((ret < 40.0f) && (ret > 85.0f)) {
        return std::nullopt;
    }

    return ret;
}