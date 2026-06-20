/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Look
*/

#ifndef ZAPPY_LOOK_HPP_
#define ZAPPY_LOOK_HPP_

#include "Player.hpp"

namespace Zappy::Server {

struct TileCoord {
    int x;
    int y;
};

[[nodiscard]] TileCoord wrapCoords(
    int x, int y, int width, int height) noexcept;

[[nodiscard]] TileCoord rotateLocalOffset(
    int dx, int dy, Direction direction) noexcept;

} // namespace Zappy::Server

#endif
