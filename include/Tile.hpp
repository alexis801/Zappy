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

namespace Zappy::Server {

class Tile {
public:
    [[nodiscard]] int get(Resource resource) const noexcept;
    void add(Resource resource, int amount) noexcept;

private:
    std::array<int, RESOURCE_COUNT> _quantities{};
};

} // namespace Zappy::Server

#endif
