/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Vision
*/

#ifndef ZAPPY_VISION_HPP_
#define ZAPPY_VISION_HPP_

#include "Player.hpp"
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

[[nodiscard]] std::vector<TileCoord> getVisibleTileCoords(
    int px, int py, Direction direction, int level,
    int width, int height) noexcept;

} // namespace Zappy::Server

#endif
