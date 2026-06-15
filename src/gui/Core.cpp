/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Core.cpp (GUI)
*/

#include "gui/Core.hpp"
#include "gui/Connection.hpp"

#include <iostream>
#include <optional>
#include <poll.h>
#include <string>

namespace Zappy::Gui {

Core::Core(const Args &args) : _connection(Socket::connectTo(args.getHost(), args.getPort()))
{
}

void Core::run()
{
    bool connected = true;

    while (connected)
        connected = update();
}

void Core::handleLine(const std::string &line)
{
    if (!_ready && line == "WELCOME") {
        _connection.send("GRAPHIC\n");
        _ready = true;
        return;
    }
    std::cout << "serveur =" << line << "\n";
}

bool Core::update()
{
    pollfd watched = {_connection.getFd(), POLLIN, 0};
    int ready = ::poll(&watched, 1, 100);

    if (ready < 0)
        return false;
    if (ready == 0)
        return true;
    if (!_connection.receive())
        return false;
    while (std::optional<std::string> line = _connection.popLine())
        handleLine(line.value());
    return true;
}

} // namespace Zappy::Gui