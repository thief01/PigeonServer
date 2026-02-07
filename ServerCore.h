#pragma once
#include <asio.hpp>
#include <vector>
#include <memory>
#include <cstdint>

#pragma pack(push, 1)
struct PlayerState {
    int32_t id;
    float x, y, z;
};
#pragma pack(pop)

class Server {
public:
    Server(asio::io_context& io, uint16_t port);

    void tick(); // tick loop, np. update world

private:
    void start_accept();

    asio::io_context& io_ctx;  // <-- referencja do io_context
    asio::ip::tcp::acceptor acceptor;
    std::vector<std::shared_ptr<asio::ip::tcp::socket>> clients;
};
