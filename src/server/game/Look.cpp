/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Look
*/

#include "Look.hpp"
#include "Macros.hpp"

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

std::vector<TileCoord> computeVisibleTileCoords(
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
        TileCoord local{lateral, -depth};
        TileCoord rotated = rotateLocalOffset(
            local.x, local.y, direction);
        TileCoord world = wrapCoords(
            px + rotated.x, py + rotated.y, width, height);

        coords.push_back(world);
    }
    return coords;
}

std::vector<std::vector<std::string>> buildTileContents(
    const Map &map, const std::vector<TileCoord> &coords) noexcept
{
    std::vector<std::vector<std::string>> tiles;
    std::size_t count = coords.size();

    tiles.resize(count);
    for (std::size_t i = 0; i < count; ++i) {
        const Tile &tile = map.at(coords[i].x, coords[i].y);

        for ([[maybe_unused]] int playerId : tile.players())
            tiles[i].emplace_back("player");
        for (std::size_t r = 0; r < RESOURCE_COUNT; ++r) {
            int amount = tile.resourceCount(static_cast<Resource>(r));

            for (int n = 0; n < amount; ++n)
                tiles[i].push_back(std::string(RESOURCE_NAMES[r]));
        }
    }
    return tiles;
}

} // namespace Zappy::Server
