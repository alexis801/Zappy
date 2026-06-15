/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Menu (GUI)
*/

#include "gui/Menu.hpp"
#include <stdexcept>

namespace Zappy::Gui {

namespace {

constexpr float CENTER_X = 1920.f / 2.f;
constexpr float TITLE_Y = 200.f;
constexpr float BUTTON_WIDTH = 300.f;
constexpr float BUTTON_HEIGHT = 80.f;
constexpr float PLAY_Y = 450.f;

void centerText(sf::Text &text, float y)
{
    text.setFillColor(sf::Color::White);
    text.setPosition(CENTER_X - text.getGlobalBounds().width / 2.f, y);
}

} // anonymous namespace

Menu::Menu()
{
    if (!_font.loadFromFile("assets/font.ttf") && !_font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf"))
        throw std::runtime_error("Menu: font not found (add a .ttf in assets/font.ttf)");
    _playButton.setSize({BUTTON_WIDTH, BUTTON_HEIGHT});
    _playButton.setPosition(CENTER_X - BUTTON_WIDTH / 2.f, PLAY_Y);
    _playButton.setFillColor(sf::Color(60, 140, 70));
}

void Menu::draw(sf::RenderWindow &window)
{
    sf::Text title("ZAPPY", _font, 120);
    sf::Text playText("Play", _font, 40);

    centerText(title, TITLE_Y);
    centerText(playText, PLAY_Y + 15.f);
    window.draw(title);
    window.draw(_playButton);
    window.draw(playText);
}

} // namespace Zappy::Gui
