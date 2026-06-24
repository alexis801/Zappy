/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Vision
*/

#include "Vision.hpp"

namespace Zappy::Server {

namespace {

int positiveModulo(int value, int size) noexcept
{
    int result = value % size;

    if (result < 0)
        result += size;
    return result;
}

TileCoord localToWorld(
    int px, int py, int dx, int dy, Direction direction,
    int width, int height) noexcept
{
    TileCoord rotated = rotateLocalOffset(dx, dy, direction);

    return wrapCoords(
        px + rotated.x, py + rotated.y, width, height);
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

std::vector<TileCoord> getVisibleTileCoords(
    int px, int py, Direction direction, int level,
    int width, int height) noexcept
{
    std::vector<TileCoord> coords;
    int total = (level + 1) * (level + 1);

    coords.reserve(static_cast<std::size_t>(total));
    for (int k = 0; k < total; ++k) {
        int depth = 0;

        while (k > depth * depth + 2 * depth)
            ++depth;
        int lateral = k - depth * depth - depth;

        coords.push_back(localToWorld(
            px, py, lateral, -depth, direction, width, height));
    }
    return coords;
}

} // namespace Zappy::Server
