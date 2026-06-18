/*
** EPITECH PROJECT, 2026
** zappy_gotenku
** File description:
** Button (GUI) - sprite cliquable a 3 etats avec un label
*/

#ifndef ZAPPY_GUI_BUTTON_HPP_
#define ZAPPY_GUI_BUTTON_HPP_

#include <SFML/Graphics.hpp>
#include <array>
#include <string>

namespace Zappy::Gui {

/**
 * @class Button
 * @brief Bouton cliquable : un cadre a 3 etats (none/hover/clicked) + un texte centre par dessus
 */
class Button {
public:
    enum class State { None, Hover, Clicked };

    Button() = default;
    Button(const Button &) = delete;
    Button &operator=(const Button &) = delete;

    bool load(const std::array<std::string, 3> &frames, const sf::Font &font, const std::string &label, sf::Vector2f position);

    void setState(State state);
    [[nodiscard]] State getState() const noexcept { return _state; }
    [[nodiscard]] bool contains(sf::Vector2f point) const;
    void draw(sf::RenderWindow &window) const;

private:
    std::array<sf::Texture, 3> _frames; // none, hover, clicked
    sf::Sprite _sprite;
    sf::Text _label;
    State _state = State::None;
};

} // namespace Zappy::Gui

#endif