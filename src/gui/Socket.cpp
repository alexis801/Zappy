/*
** EPITECH PROJECT, 2026
** zappy_gotenku
** File description:
** Socket.cpp
*/

#include "gui/Socket.hpp"

#include <unistd.h>

namespace Zappy::Gui {

Socket::Socket(int fd) noexcept : _fd(fd)
{
}

Socket::~Socket()
{
    if (_fd >= 0)
        ::close(_fd);
}

} // namespace Zappy::Gui