/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Socket
*/

#include "Socket.hpp"
#include <cstdint>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdexcept>
#include <sys/socket.h>
#include <unistd.h>

namespace Zappy::Server {

namespace {

void setNonBlocking(int fd)
{
    int flags = ::fcntl(fd, F_GETFL, 0);

    if (flags < 0 || ::fcntl(fd, F_SETFL, flags | O_NONBLOCK) < 0)
        throw std::runtime_error("fcntl: passage en non-bloquant impossible");
}

} // namespace

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

Socket Socket::listenOn(int port)
{
    int fd = ::socket(AF_INET, SOCK_STREAM, 0);
    int yes = 1;
    sockaddr_in addr = {};

    if (fd < 0)
        throw std::runtime_error("socket: creation impossible");
    Socket result(fd);
    ::setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(static_cast<uint16_t>(port));
    if (::bind(fd, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) < 0)
        throw std::runtime_error("bind: port indisponible");
    if (::listen(fd, SOMAXCONN) < 0)
        throw std::runtime_error("listen: echec");
    setNonBlocking(fd);
    return result;
}

Socket Socket::acceptClient() const
{
    int clientFd = ::accept(_fd, nullptr, nullptr);

    if (clientFd < 0)
        return Socket();
    setNonBlocking(clientFd);
    return Socket(clientFd);
}

} // namespace Zappy::Server
