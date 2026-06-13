/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Tile
*/

#include "Tile.hpp"

namespace Zappy::Server {

int Tile::get(Resource resource) const noexcept
{
    return _quantities[static_cast<std::size_t>(resource)];
}

} // namespace Zappy::Server
