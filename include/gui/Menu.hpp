/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Menu.hpp (GUI)
*/

#ifndef ZAPPY_GUI_MENU_HPP_
#define ZAPPY_GUI_MENU_HPP_

#include <SFML/Graphics.hpp>

namespace Zappy::Gui {
/**
 * @class Menu
 * @brief Ecran accueil 
 */
class Menu {
public:
    enum class ButtonAction { None, Play, Quit};

    Menu();

    void draw(sf::RenderWindow &window);
    [[nodiscard]] ButtonAction handleClick(sf::Vector2f position) const;


private:
    sf::Font _font;
    sf::Texture _backgroundTexture;
    sf::Sprite _background;
    sf::RectangleShape _playButton;
    sf::RectangleShape _quitButton;
};

} // namespace Zappy::Gui

#endif // ZAPPY_GUI_MENU_HPP_