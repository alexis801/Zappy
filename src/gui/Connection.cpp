/*
** EPITECH PROJECT, 2026
** zappy_gotenku
** File description:
** Connection (GUI)
*/

#include "gui/Connection.hpp"

#include <utility>
#include <array>
#include <sys/socket.h>

namespace Zappy::Gui {

namespace {

constexpr std::size_t BUFFER_CHUNK_SIZE = 4096;

} // anonymous namespace

Connection::Connection(Socket socket) noexcept : _socket(std::move(socket))
{
}

void Connection::feed(std::string_view data)
{
    _readBuffer.append(data);
}

std::optional<std::string> Connection::popLine()
{
    std::size_t pos = _readBuffer.find('\n');

    if (pos == std::string::npos)
        return std::nullopt;
    std::string line = _readBuffer.substr(0, pos);

    _readBuffer.erase(0, pos + 1);
    return line;
}

bool Connection::receive()
{
    std::array<char, BUFFER_CHUNK_SIZE> buffer = {};
    ssize_t count_bytes = recv(_socket.getFd(), buffer.data(), buffer.size(), 0);

    if (count_bytes <= 0)
        return false;
    feed(std::string_view(buffer.data(), static_cast<std::size_t>(count_bytes)));
    return true;
}

} // namespace Zappy::Gui