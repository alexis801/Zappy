/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Look
*/

#ifndef ZAPPY_LOOK_HPP_
#define ZAPPY_LOOK_HPP_

#include "Map.hpp"
#include "Player.hpp"
#include "Vision.hpp"
#include <string>
#include <vector>

namespace Zappy::Server {

[[nodiscard]] std::vector<std::vector<std::string>> buildTileContents(
    const Map &map, const std::vector<TileCoord> &coords) noexcept;

[[nodiscard]] std::string formatLookResponse(
    const std::vector<std::vector<std::string>> &tiles) noexcept;

[[nodiscard]] std::string performLook(
    const Player &player, const Map &map) noexcept;

} // namespace Zappy::Server

#endif
