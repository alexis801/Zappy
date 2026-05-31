/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Client
*/

#include "Client.hpp"
#include <array>
#include <sys/socket.h>
#include <utility>

namespace Zappy::Server {

namespace {
constexpr std::size_t READ_CHUNK = 4096;
}

Client::Client(Socket socket) noexcept : _socket(std::move(socket))
{
}

bool Client::receive()
{
    std::array<char, READ_CHUNK> buffer;
    ssize_t count = ::recv(_socket.getFd(), buffer.data(), buffer.size(), 0);

    if (count <= 0)
        return false;
    feed(std::string_view(buffer.data(), static_cast<std::size_t>(count)));
    return true;
}

void Client::feed(std::string_view data)
{
    _readBuffer.append(data);
}

std::optional<std::string> Client::popLine()
{
    std::size_t pos = _readBuffer.find('\n');

    if (pos == std::string::npos)
        return std::nullopt;
    std::string line = _readBuffer.substr(0, pos);

    _readBuffer.erase(0, pos + 1);
    return line;
}

} // namespace Zappy::Server
