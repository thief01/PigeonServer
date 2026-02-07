#include <thread>
#include "Core/ServerCore.h"

int main() {
    asio::io_context io;
    Config config = Config("config.ini");
    Server server(io, config);

    // Asio async w osobnym wątku
    std::thread asioThread([&io]() { io.run(); });

    // Tick loop ~14ms (70 FPS)
    while (true) {
        server.tick();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000 / config.tickRate));
    }

    asioThread.join();
    return 0;
}
