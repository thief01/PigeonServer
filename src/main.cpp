#include <thread>
#include "../ServerCore.h"

int main() {
    asio::io_context io;

    Server server(io, 12345);

    // Asio async w osobnym wątku
    std::thread asioThread([&io]() { io.run(); });

    // Tick loop ~14ms (70 FPS)
    while (true) {
        server.tick();
        std::this_thread::sleep_for(std::chrono::milliseconds(14));
    }

    asioThread.join();
    return 0;
}
