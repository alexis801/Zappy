/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Socket
*/

#include "Socket.hpp"
#include <cerrno>
#include <cstdint>
#include <cstring>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdexcept>
#include <string>
#include <string_view>
#include <sys/socket.h>
#include <unistd.h>

namespace Zappy::Server {

namespace {

[[nodiscard]] std::string buildSystemError(std::string_view prefix)
{
    return std::string(prefix) + ": " + std::strerror(errno);
}

void setNonBlocking(int fd)
{
    int flags = ::fcntl(fd, F_GETFL, 0);

    if (flags < 0 || ::fcntl(fd, F_SETFL, flags | O_NONBLOCK) < 0)
        throw std::runtime_error(
            buildSystemError("fcntl(O_NONBLOCK) failed"));
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
        throw std::runtime_error(buildSystemError("socket() failed"));
    Socket result(fd);
    ::setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(static_cast<uint16_t>(port));
    if (::bind(fd, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) < 0)
        throw std::runtime_error(
            buildSystemError("bind() failed on port " + std::to_string(port)));
    if (::listen(fd, SOMAXCONN) < 0)
        throw std::runtime_error(buildSystemError("listen() failed"));
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

bool Socket::write(std::string_view data)
{
    while (!data.empty()) {
        ssize_t count = ::send(_fd, data.data(), data.size(), MSG_NOSIGNAL);

        if (count <= 0)
            return false;
        data.remove_prefix(static_cast<std::size_t>(count));
    }
    return true;
}

} // namespace Zappy::Server
