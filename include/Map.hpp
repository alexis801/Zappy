/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Map
*/

#ifndef ZAPPY_MAP_HPP_
#define ZAPPY_MAP_HPP_

#include "Tile.hpp"
#include <vector>

namespace Zappy::Server {

class Map {
public:
    Map(int width, int height);

    [[nodiscard]] int getWidth() const noexcept { return _width; }
    [[nodiscard]] int getHeight() const noexcept { return _height; }

    [[nodiscard]] Tile &at(int x, int y) noexcept;
    [[nodiscard]] const Tile &at(int x, int y) const noexcept;

private:
    [[nodiscard]] int normalize(int value, int size) const noexcept;

    int _width;
    int _height;
    std::vector<Tile> _tiles;
};

} // namespace Zappy::Server

#endif
