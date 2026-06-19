/*
** EPITECH PROJECT, 2026
** zappy_gotenku
** File description:
** Button (GUI)
*/

#include "gui/Button.hpp"

namespace Zappy::Gui {

namespace {

constexpr unsigned int LABEL_SIZE = 40;

void centerLabel(sf::Text &label, const sf::Sprite &sprite)
{
    sf::FloatRect text = label.getLocalBounds();
    sf::FloatRect box = sprite.getGlobalBounds();

    label.setOrigin(text.left + text.width / 2.f, text.top + text.height / 2.f);
    label.setPosition(box.left + box.width / 2.f, box.top + box.height / 2.f);
}

} // anonymous namespace

bool Button::load(const std::array<std::string, 3> &frames, const sf::Font &font,
    const std::string &label, sf::Vector2f position)
{
    for (std::size_t i = 0; i < _frames.size(); ++i)
        if (!_frames[i].loadFromFile(frames[i]))
            return false;
    _sprite.setTexture(_frames[0], true);
    _sprite.setPosition(position);

    _label.setFont(font);
    _label.setString(label);
    _label.setCharacterSize(LABEL_SIZE);
    _label.setFillColor(sf::Color::White);
    centerLabel(_label, _sprite);
    setState(State::None);
    return true;
}

void Button::setState(State state)
{
    _state = state;
    _sprite.setTexture(_frames[static_cast<std::size_t>(state)], true);
}

bool Button::contains(sf::Vector2f point) const
{
    return _sprite.getGlobalBounds().contains(point);
}

void Button::draw(sf::RenderWindow &window) const
{
    window.draw(_sprite);
    window.draw(_label);
}

} // namespace Zappy::Gui