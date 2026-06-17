/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Menu.hpp (GUI)
*/

#ifndef ZAPPY_GUI_MENU_HPP_
#define ZAPPY_GUI_MENU_HPP_

#include "gui/Button.hpp"

#include <SFML/Graphics.hpp>
#include <array>

namespace Zappy::Gui {
/**
 * @class Menu
 * @brief Ecran accueil 
 */
class Menu {
public:
    enum class ButtonAction { None, Play, Options, Quit };

    Menu();

    void updateHover(sf::Vector2f mouse);
    void press(sf::Vector2f mouse);
    [[nodiscard]] ButtonAction release(sf::Vector2f mouse);
    void draw(sf::RenderWindow &window);


private:
    sf::Font _font;
    sf::Texture _backgroundTexture;
    sf::Sprite _background;
    sf::Texture _titleTexture;
    std::array<Button, 3> _buttons;
};

} // namespace Zappy::Gui

#endif // ZAPPY_GUI_MENU_HPP_