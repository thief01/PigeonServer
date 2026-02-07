cmake_minimum_required(VERSION 4.1)
project(Pigeon)

set(CMAKE_CXX_STANDARD 20)

add_executable(Pigeon main.cpp
        ServerCore.h
        ServerCore.cpp
        PlayerState.h)

# Ścieżka do folderu zawierającego asio.hpp
set(ASIO_PATH "D:/libs/asio-1.36.0/include")

# Dodajemy do include path dla targetu Pigeon
target_include_directories(Pigeon PRIVATE ${ASIO_PATH})

# Mówimy Asio, że standalone
target_compile_definitions(Pigeon PRIVATE ASIO_STANDALONE _WIN32_WINNT=0x0A00)
