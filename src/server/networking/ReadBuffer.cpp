/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** ReadBuffer
*/

#include "ReadBuffer.hpp"

namespace Zappy::Server {

void ReadBuffer::feed(std::string_view data)
{
    _bytes.append(data);
}

std::optional<std::string> ReadBuffer::popLine()
{
    std::size_t pos = _bytes.find('\n');

    if (pos == std::string::npos)
        return std::nullopt;
    std::string line = _bytes.substr(0, pos);

    _bytes.erase(0, pos + 1);
    return line;
}

} // namespace Zappy::Server
