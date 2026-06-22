/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Look
*/

#include "Look.hpp"
#include "Macros.hpp"

namespace Zappy::Server {

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

std::string formatLookResponse(
    const std::vector<std::vector<std::string>> &tiles) noexcept
{
    std::string result = "[";

    for (std::size_t i = 0; i < tiles.size(); ++i) {
        if (i > 0)
            result += ", ";
        for (std::size_t j = 0; j < tiles[i].size(); ++j) {
            if (j > 0)
                result += ' ';
            result += tiles[i][j];
        }
    }
    result += ']';
    return result;
}

std::string performLook(const Player &player, const Map &map) noexcept
{
    std::vector<TileCoord> coords = getVisibleTileCoords(
        player.getX(), player.getY(), player.getDirection(),
        player.getLevel(), map.getWidth(), map.getHeight());
    std::vector<std::vector<std::string>> contents =
        buildTileContents(map, coords);

    return formatLookResponse(contents);
}

} // namespace Zappy::Server
