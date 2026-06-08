/*
** EPITECH PROJECT, 2026
** zappy_gotenku
** File description:
** Socket.hpp (GUI)
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
    Socket() noexcept = default;
    explicit Socket(int fd) noexcept;
    ~Socket();

    Socket(const Socket &) = delete;
    Socket &operator=(const Socket &) = delete;
    Socket(Socket &&other) noexcept;
    Socket &operator=(Socket &&other) noexcept;

    [[nodiscard]] int getFd() const noexcept { return _fd; }
    [[nodiscard]] bool isValid() const noexcept { return _fd >= 0; }

private:
    int _fd = -1;
};

} // namespace Zappy::Gui

#endif // ZAPPY_GUI_SOCKET_HPP_