#include "../ServerCore.h"
#include <iostream>

Server::Server(asio::io_context& io, uint16_t port)
    : io_ctx(io),
      acceptor(io_ctx, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)) {
    start_accept();
}

void Server::tick() {
    // Tutaj np. update wszystkich graczy
    for (auto& client : clients) {
        if (client->is_open()) {
            // Możesz wysłać dane do klienta
        }
    }
}

void Server::start_accept() {
    auto socket = std::make_shared<asio::ip::tcp::socket>(io_ctx); // <-- poprawnie io_context
    acceptor.async_accept(*socket, [this, socket](const asio::error_code& ec) {
        if (!ec) {
            std::cout << "Client connected!\n";
            clients.push_back(socket);
        }
        start_accept(); // akceptuj kolejnego klienta
    });
}
