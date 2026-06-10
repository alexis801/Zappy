/*
** EPITECH PROJECT, 2026
** zappy_gotenku
** File description:
** Socket.cpp (GUI)
*/

#include "gui/Socket.hpp"

#include <string>
#include <stdexcept>
#include <unistd.h>
#include <fcntl.h>
#include <netdb.h>
#include <sys/socket.h>

namespace Zappy::Gui {

namespace {

void setNoBlocking (int fd)
{   
    // make fd non block mode & raise error
    int flags = fcntl(fd, F_GETFL, 0);

    if (flags < 0 || ::fcntl(fd, F_SETFL, flags | O_NONBLOCK) < 0)
        throw std::runtime_error("fcntl: Non blocking change impossible");

}

} // anonymous namespace

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

Socket Socket::connectTo(std::string_view host, int port)
{
    addrinfo hints = {};
    addrinfo *infos = nullptr;
    std::string hostTxt(host);
    std::string portTxt = std::to_string(port);

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if (::getaddrinfo(hostTxt.c_str(), portTxt.c_str(), &hints, &infos) != 0)
        throw std::runtime_error("getadddrinfo: host not found" + hostTxt);

    int fd = ::socket(infos->ai_family, infos->ai_socktype, infos->ai_protocol);
    if (fd < 0) {
        ::freeaddrinfo(infos);
        throw std::runtime_error("socket creation impossible");
    }
    Socket result(fd);
    if (::connect(fd, infos->ai_addr, infos->ai_addrlen) < 0) {
        ::freeaddrinfo(infos);
        throw std::runtime_error("connection to server impossible");
    }
    ::freeaddrinfo(infos);
    setNoBlocking(fd);
    return result;
}

} // namespace Zappy::Gui