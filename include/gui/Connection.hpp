/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Connection (GUI)
*/

#ifndef ZAPPY_GUI_CONNECTION_HPP_
#define ZAPPY_GUI_CONNECTION_HPP_

#include "gui/Socket.hpp"

namespace Zappy::Gui {
/**
*@class Connection
*@brief Lien vers le serveur socket et buffer de lecture
*/
class Connection {
public:
    explicit Connection(Socket socket) noexcept;

private:
    Socket _socket;
};

} // namespace Zappy::Gui

#endif //ZAPPY_GUI_CONNECTION_HPP_