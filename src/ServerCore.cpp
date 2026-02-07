#include "ServerCore.h"
#include <iostream>

#include "PlayerState.h"

Server::Server(asio::io_context& io, uint16_t port)
    : io_ctx(io),
      acceptor(io_ctx, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)) {
    start_accept();
}

void Server::tick() {
    cleanup_dead_clients();

    std::lock_guard<std::mutex> lock(clients_mutex);
    for (auto& client : clients) {
        if (client->is_open()) {
            // Tutaj async_write, nie synchroniczne operacje!
            PlayerState state{1, 10.0f, 20.0f, 30.0f};
            asio::async_write(*client, asio::buffer(&state, sizeof(state)),
                [](const asio::error_code& ec, std::size_t) {
                    if (ec) std::cerr << "Send failed: " << ec.message() << "\n";
                });
        }
    }
}

void Server::start_accept() {
    auto socket = std::make_shared<asio::ip::tcp::socket>(io_ctx);
    acceptor.async_accept(*socket, [this, socket](const asio::error_code& ec) {
        if (!ec) {
            std::cout << "Client connected!\n";
            {
                std::lock_guard<std::mutex> lock(clients_mutex);
                clients.push_back(socket);
            }
        }
        start_accept();
    });
}

void Server::cleanup_dead_clients() {
    std::lock_guard<std::mutex> lock(clients_mutex);
    clients.erase(
        std::remove_if(clients.begin(), clients.end(),
            [](auto& s) { return !s->is_open(); }),
        clients.end()
    );
}