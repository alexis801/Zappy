/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Map
*/

#include "Map.hpp"

namespace Zappy::Server {

Map::Map(int width, int height)
    : _width(width), _height(height), _tiles(width * height)
{
}

} // namespace Zappy::Server
