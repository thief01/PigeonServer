//
// Created by pst12 on 7.02.2026.
//

#include "Config.h"

#include <fstream>
#include <sstream>

#include "Logger/Logger.h"


Config::Config(const std::string &path) {
    std::ifstream file(path);
    std::string line;

    while (std::getline(file, line)) {
        auto pos = line.find("=");
        if (pos == std::string::npos) continue;;

        auto key = line.substr(0, pos);
        auto val = line.substr(pos + 1);

        if (key == "port") port = std::stoi(val);
        if (key == "tick_rate") tickRate = std::stoi(val);
    }
}
