/*
** EPITECH PROJECT, 2026
** zappy_gotenku
** File description:
** Socket.hpp
*/

#ifndef ZAPPY_GUI_SOCKET_HPP_
#define ZAPPY_GUI_SOCKET_HPP_

namespace Zappy::Gui {

/**
 * @class Socket
 * @brief Socket descriptor and auto close
 */

class Socket {
public:
    Socket noexcept= default;
    explicit Socket(int fd) noexcept;
    ~Socket();

private:
    int _fd = -1;
};

} // namespace Zappy::Gui

#endif // ZAPPY_GUI_SOCKET_HPP_