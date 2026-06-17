/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Factory.hpp (GUI) fabrique des objets sfml
*/

#ifndef ZAPPY_GUI_FACTORY_HPP_
#define ZAPPY_GUI_FACTORY_HPP_

#include <SFML/Graphics.hpp>
#include <string>

namespace Zappy::Gui::factory {

// create texte to draw
inline sf::Text text(const sf::Font &font, const std::string &content, unsigned int size, sf::Color color = sf::Color::White)
{
    sf::Text label(content, font, size);

    label.setFillColor(color);
    return label;
}

// create rectangle to draw
inline sf::RectangleShape rect(sf::Vector2f size, sf::Vector2f position, sf::Color color)
{
    sf::RectangleShape shape(size);

    shape.setPosition(position);
    shape.setFillColor(color);
    return shape;
}

// create cercle to draw
inline sf::CircleShape circle(float radius, sf::Vector2f position, sf::Color color)
{
    sf::CircleShape shape(radius);

    shape.setPosition(position);
    shape.setFillColor(color);
    return shape;
}

// center a text horizontaly
inline void centerText(sf::Text &label, float center, float y)
{
    label.setPosition(center - label.getGlobalBounds().width / 2.f, y);
}

} // namespace Zappy::Gui::factory

#endif // ZAPPY_GUI_FACTORY_HPP_