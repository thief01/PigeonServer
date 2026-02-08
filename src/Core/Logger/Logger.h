//
// Created by pst12 on 7.02.2026.
//
#pragma once
#ifndef PIGEON_LOGGER_H
#define PIGEON_LOGGER_H
#include <fstream>
#include <mutex>
#include <queue>
#include <string>

#define LOG_TRACE(msg) Logger::instance().log(LogLevel::Trace, msg);
#define LOG_DEBUG(msg) Logger::instance().log(LogLevel::Debug, msg);
#define LOG_INFO(msg) Logger::instance().log(LogLevel::Info, msg);
#define LOG_WARN(msg) Logger::instance().log(LogLevel::Warn, msg);
#define LOG_ERROR(msg) Logger::instance().log(LogLevel::Error, msg);
#define LOG_FATAL(msg) Logger::instance().log(LogLevel::Fatal, msg);

enum class LogLevel {
    Trace, Debug, Info, Warn, Error, Fatal
};

struct LogMessage {
    LogLevel level;
    std::string text;
};

inline const char* LogLevelToSTring(LogLevel logLevel) {
    switch (logLevel) {
        case LogLevel::Trace: return "[TRACE]";
        case LogLevel::Debug: return "[DEBUG]";
        case LogLevel::Info: return "[INFO]";
        case LogLevel::Warn: return "[WARN]";
        case LogLevel::Error: return "[ERROR]";
        case LogLevel::Fatal: return "[FATAL]";
    }
}


class Logger {
public:
    static Logger& instance();

    void log(LogLevel level, std::string msg);

    void start();
    void stop();

private:
    Logger();
    void worker();

    std::queue<LogMessage> queue;
    std::mutex mutex;
    std::atomic<bool> running{false};
    std::thread thread;
    std::ofstream file;
};


#endif //PIGEON_LOGGER_H