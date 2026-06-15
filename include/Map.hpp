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

private:
    int _width;
    int _height;
    std::vector<Tile> _tiles;
};

} // namespace Zappy::Server

#endif
