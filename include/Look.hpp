/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Look
*/

#ifndef ZAPPY_LOOK_HPP_
#define ZAPPY_LOOK_HPP_

#include "Player.hpp"
#include "Map.hpp"
#include <string>
#include <vector>

namespace Zappy::Server {

struct TileCoord {
    int x;
    int y;
};

[[nodiscard]] TileCoord wrapCoords(
    int x, int y, int width, int height) noexcept;

[[nodiscard]] TileCoord rotateLocalOffset(
    int dx, int dy, Direction direction) noexcept;

[[nodiscard]] std::vector<TileCoord> computeVisibleTileCoords(
    int px, int py, Direction direction, int level,
    int width, int height) noexcept;

[[nodiscard]] std::vector<std::vector<std::string>> buildTileContents(
    const Map &map, const std::vector<TileCoord> &coords) noexcept;

[[nodiscard]] std::string formatLookResponse(
    const std::vector<std::vector<std::string>> &tiles) noexcept;

[[nodiscard]] std::string performLook(
    const Player &player, const Map &map) noexcept;

} // namespace Zappy::Server

#endif
