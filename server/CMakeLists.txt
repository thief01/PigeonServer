cmake_minimum_required(VERSION 4.1)
project(Pigeon)

set(CMAKE_CXX_STANDARD 20)

# Automatycznie znajdź wszystkie .cpp
file(GLOB SOURCES "src/*.cpp")
# Lub rekurencyjnie:
# file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(Pigeon
        src/main.cpp
        src/Core/Config.cpp
        src/Core/TcpServer.cpp
        src/Core/Logger/Logger.cpp
        src/Core/Logger/Logger.h
        src/Core/ClientSession.h
        src/Core/Packets/Packet.h
        src/Core/Packets/DataManager.cpp
        src/Core/Packets/DataManager.h
)


# ASIO
set(ASIO_PATH "D:/libs/asio-1.36.0/include")
target_include_directories(Pigeon PRIVATE src ${ASIO_PATH})
target_compile_definitions(Pigeon PRIVATE ASIO_STANDALONE _WIN32_WINNT=0x0A00)

# Windows
#if(WIN32)
#    target_link_libraries(Pigeon ws2_32 wsock32 mswsock)
#endif()