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

int Map::normalize(int value, int size) const noexcept
{
    int result = value % size;

    if (result < 0)
        result += size;
    return result;
}

} // namespace Zappy::Server
