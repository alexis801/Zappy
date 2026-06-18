/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** World.hpp (GUI)
*/

#ifndef ZAPPY_GUI_WORLD_HPP_
#define ZAPPY_GUI_WORLD_HPP_

#include "Macros.hpp"

#include <array>
#include <vector>

namespace Zappy::Gui {

/**
 * @class World
 * @brief Copie locale du monde taille de la carte + cases (ressources)
 */

class World {
public:

    // Une case = 7 quantités de ressources (index par resources)
    using Tile = std::array<int, Zappy::RESOURCE_COUNT>;

    [[nodiscard]] int getWidth() const noexcept { return _width; }
    [[nodiscard]] int getHeight() const noexcept { return _height; }

    // Fixe la taille et alloue/ re alloue les case a 0
    void setSize(int width, int height)
    {
        _width = width;
        _height = height;
        _tiles.assign(static_cast<std::size_t>(width) * height, Tile{});
    }

    // Met a jour le contenue d'une case (ignore si hors)
    void setTile(int x, int y, const Tile &tile)
    {
        if (contains(x, y))
            _tiles[index(x, y)] = tile;
    }

    [[nodiscard]] const Tile &getTile(int x, int y) const
    {
        return _tiles[index(x, y)];
    }

private:

    [[nodiscard]] bool contains(int x, int y) const noexcept
    {
        return x >= 0 && y >= 0 && x < _width && y < _height;
    }

        [[nodiscard]] std::size_t index(int x, int y) const noexcept
    {
        return static_cast<std::size_t>(y) * _width + x;
    }

    int _width = 0;
    int _height = 0;
    std::vector<Tile> _tiles;
};

} // namespace Zappy::Gui

#endif // ZAPPY_GUI_WORLD_HPP_