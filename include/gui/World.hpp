/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** World.hpp (GUI)
*/

#ifndef ZAPPY_GUI_WORLD_HPP_
#define ZAPPY_GUI_WORLD_HPP_

namespace Zappy::Gui {

/**
 * @class World
 * @brief Copie locale du monde pour l'instant juste la taille de carte
 */

 class World {
public:

    [[nodiscard]] int getWidth() const noexcept { return _width; }
    [[nodiscard]] int getHeight() const noexcept { return _height; }

    void setSize(int width, int height) noexcept
    {
        _width = width;
        _height = height;
    }

private:
    int _width = 0;
    int _height = 0;
};

} // namespace Zappy::Gui

#endif // ZAPPY_GUI_WORLD_HPP_