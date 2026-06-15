/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Renderer.hpp (GUI)
*/

#ifndef ZAPPY_GUI_RENDERER_HPP_
#define ZAPPY_GUI_RENDERER_HPP_

#include "gui/World.hpp"
#include "gui/Menu.hpp"

#include <cstddef>
#include <SFML/Graphics.hpp>

namespace Zappy::Gui {
/**
 * @class Renderer
 * @brief Fenetre SFML draw le world
 */
class Renderer {
public:
    Renderer();

    [[nodiscard]] bool isOpen() const;
    void handleEvents();
    void draw(const World &world);
    void drawMenu(Menu &menu);


private:
    void drawGrid(const World &world);
    void drawResources(const World &world);
    void drawTileResources(const World::Tile &tile, int x, int y, float cell);
    void drawResourceDot(std::size_t resource, int x, int y, float cell);

    sf::RenderWindow _window;
};

} // namespace Zappy::Gui

#endif // ZAPPY_GUI_RENDERER_HPP_