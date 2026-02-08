//
// Created by pst12 on 7.02.2026.
//

#include "Logger.h"

#include <chrono>
#include <iostream>

Logger &Logger::instance() {
    static Logger logger;
    return logger;
}

Logger::Logger() {
    file.open("serve.log", std::ios::app);
}

void Logger::start() {
    running =true;
    thread = std::thread(&Logger::worker, this);
}

void Logger::stop() {
    running = false;
    if (thread.joinable()) {
        thread.join();
    }
}

void Logger::log(LogLevel level, std::string msg) {
    std::lock_guard lock(mutex);
    queue.emplace(LogMessage{level, std::move(msg)});
}


void Logger::worker() {
    while (running) {
        LogMessage msg;
        {
            std::lock_guard<std::mutex> lock(mutex);
            if (!queue.empty()) {
                msg = queue.front();
                queue.pop();
            }
            else {
                msg.text.clear();
            }
        }

        if (!msg.text.empty()) {
            auto now = std::chrono::system_clock::now();
            auto time = std::chrono::system_clock::to_time_t(now);
            std::ostringstream oss;
            oss << std::put_time(std::localtime(&time), "%F %T") << " " << LogLevelToSTring(msg.level) << ": " << msg.text << "\n";
            std::string logRow = oss.str();
            file << logRow;

            std::cout << logRow;
        }

        std::this_thread::sleep_for(std::chrono::microseconds(1));
    }
}
