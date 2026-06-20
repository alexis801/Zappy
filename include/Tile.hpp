/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Tile
*/

#ifndef ZAPPY_TILE_HPP_
#define ZAPPY_TILE_HPP_

#include "Macros.hpp"
#include <array>
#include <cstddef>
#include <vector>

namespace Zappy::Server {

class Tile {
public:
    [[nodiscard]] int get(Resource resource) const noexcept;
    void add(Resource resource, int amount) noexcept;
    void remove(Resource resource, int amount) noexcept;
    [[nodiscard]] bool isEmpty() const noexcept;
    [[nodiscard]] int total() const noexcept;

    void addPlayer(int playerId) noexcept;
    void removePlayer(int playerId) noexcept;
    [[nodiscard]] const std::vector<int> &players() const noexcept;
    [[nodiscard]] int resourceCount(Resource resource) const noexcept;

private:
    std::array<int, RESOURCE_COUNT> _quantities{};
    std::vector<int> _players;
};

} // namespace Zappy::Server

#endif
