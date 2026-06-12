/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Renderer.hpp (GUI)
*/

#ifndef ZAPPY_GUI_RENDERER_HPP_
#define ZAPPY_GUI_RENDERER_HPP_

#include "gui/World.hpp"

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
    void handleEvent();
    void draw(const World &world);


private:
    sf::RenderWindow _window;
};

} // namespace Zappy::Gui

#endif // ZAPPY_GUI_RENDERER_HPP_