/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Look
*/

#include "Look.hpp"

namespace Zappy::Server {

namespace {

int positiveModulo(int value, int size) noexcept
{
    int result = value % size;

    if (result < 0)
        result += size;
    return result;
}

} // namespace

TileCoord wrapCoords(int x, int y, int width, int height) noexcept
{
    return {
        positiveModulo(x, width),
        positiveModulo(y, height),
    };
}

TileCoord rotateLocalOffset(int dx, int dy, Direction direction) noexcept
{
    switch (direction) {
    case Direction::NORTH:
        return {dx, dy};
    case Direction::EAST:
        return {-dy, dx};
    case Direction::SOUTH:
        return {-dx, -dy};
    case Direction::WEST:
        return {dy, -dx};
    }
    return {dx, dy};
}

} // namespace Zappy::Server
