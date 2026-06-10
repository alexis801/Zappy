/*
** EPITECH PROJECT, 2026
** zappy_gotenku
** File description:
** Socket.cpp (GUI)
*/

#include "gui/Socket.hpp"

#include <string>
#include <memory>
#include <stdexcept>
#include <unistd.h>
#include <fcntl.h>
#include <netdb.h>
#include <sys/socket.h>

namespace Zappy::Gui {

namespace {

// make fd non block mode & raise error
void setNoBlocking (int fd)
{
    int flags = fcntl(fd, F_GETFL, 0);

    if (flags < 0 || ::fcntl(fd, F_SETFL, flags | O_NONBLOCK) < 0)
        throw std::runtime_error("fcntl: Non blocking change impossible");

}

// resout le host:port en adresse
addrinfo *resolveAdress(std::string_view host, int port)
{
    addrinfo hints = {};
    addrinfo *infos = nullptr;
    std::string hostTxt(host);
    std::string portTxt = std::to_string(port);

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if (::getaddrinfo(hostTxt.c_str(), portTxt.c_str(), &hints, &infos) != 0)
        throw std::runtime_error("getadddrinfo: host not found" + hostTxt);
    return infos;
}

// create Socket et se co a l'adresse trouve pr resolve adres
Socket connectToAdress(const addrinfo &adress)
{
    int fd = ::socket(adress.ai_family, adress.ai_socktype, adress.ai_protocol);

    if (fd < 0)
        throw std::runtime_error("socket creation impossible");

    Socket result(fd);
    if (::connect(fd, adress.ai_addr, adress.ai_addrlen) < 0)
        throw std::runtime_error("connect: connexion au serveur impossible");
    setNoBlocking(fd);
    return result;
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
    std::unique_ptr<addrinfo, decltype(&::freeaddrinfo)> infos(resolveAdress(host, port), &::freeaddrinfo);

    return connectToAdress(*infos);
}

bool Socket::write(std::string_view data)
{
    while (!data.empty()) {
        size_t count = ::send(_fd, data.data(), data.size(), MSG_NOSIGNAL);
        if (count < 0)
            return false;
        data.remove_prefix(static_cast<std::size_t>(count));
    }
    return true;
}

} // namespace Zappy::Gui