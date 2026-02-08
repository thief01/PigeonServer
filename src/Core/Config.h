//
// Created by pst12 on 7.02.2026.
//

#ifndef PIGEON_CONFIG_H
#define PIGEON_CONFIG_H
#include <string>


class Config {
public:
    int port = 12345;
    int tickRate = 5;
    Config(const std::string& path);
};


#endif //PIGEON_CONFIG_H