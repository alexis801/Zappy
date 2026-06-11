/*
** EPITECH PROJECT, 2026
** zappy_gotenku
** File description:
** Connection (GUI)
*/

#ifndef ZAPPY_GUI_CONNECTION_HPP_
#define ZAPPY_GUI_CONNECTION_HPP_

#include "gui/Socket.hpp"
#include <optional>
#include <string>
#include <string_view>

namespace Zappy::Gui {

/**
 * @class Connection
 * @brief Lien vers le serveur : une socket et un buffer de lecture
 */
class Connection {
public:
    explicit Connection(Socket socket) noexcept;

    [[nodiscard]] int getFd() const noexcept { return _socket.getFd(); }

    /**
     * @brief Ajoute des data au buffer
     */
    void feed(std::string_view data);

    /**
     * @brief Retire la prochaine ligne complete, si elle existe
     */
    [[nodiscard]] std::optional<std::string> popLine();

private:
    Socket _socket;
    std::string _readBuffer;
};

} // namespace Zappy::Gui

#endif // ZAPPY_GUI_CONNECTION_HPP_