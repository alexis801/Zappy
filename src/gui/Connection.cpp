/*
** EPITECH PROJECT, 2026
** zappy_gotenku
** File description:
** Connection (GUI)
*/

#include "gui/Connection.hpp"

#include <utility>

namespace Zappy::Gui {

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

} // namespace Zappy::Gui