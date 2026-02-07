#pragma once
#include <asio.hpp>
#include <vector>
#include <memory>
#include <mutex>
#include <cstdint>

class Server {
public:
    Server(asio::io_context& io, uint16_t port);
    void tick();

private:
    void start_accept();
    void cleanup_dead_clients();

    asio::io_context& io_ctx;
    asio::ip::tcp::acceptor acceptor;

    std::vector<std::shared_ptr<asio::ip::tcp::socket>> clients;
    std::mutex clients_mutex;  // <-- WYMAGANE
};