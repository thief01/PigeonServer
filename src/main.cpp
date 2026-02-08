#include <thread>
#include "Core/TcpServer.h"
#include "Core/Logger/Logger.h"

int main() {
    Logger::instance().start();

    asio::io_context io;
    Config config = Config("config.ini");
    TcpServer server(io, config);

    std::thread asioThread([&io]() { io.run(); });

    while (true) {
        server.tick();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000 / config.tickRate));
    }

    asioThread.join();
    return 0;
}
