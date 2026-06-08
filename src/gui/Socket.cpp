/*
** EPITECH PROJECT, 2026
** zappy_gotenku
** File description:
** Socket.cpp (GUI)
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

Socket::Socket(Socket &&other) noexcept : _fd(other._fd)
{
    other._fd = -1;
}

Socket &Socket::operator=(Socket &&other) noexcept
{
    if (this != &other) {
        if (_fd >= 0)
            ::close(_fd);
        _fd = other._fd;
        other._fd = -1;
    }
    return *this;
}

} // namespace Zappy::Gui