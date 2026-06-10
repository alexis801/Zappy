/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Connection (GUI)
*/

#include "gui/Connection.hpp"

#include <utility>

namespace Zappy::Gui {


Connection::Connection(Socket Socket) noexcept : _socket(std::move(Socket))
{
}

} //namespace Zappy::Gui