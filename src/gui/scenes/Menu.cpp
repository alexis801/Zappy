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
constexpr float QUIT_Y = 580.f;

void centerText(sf::Text &text, float y)
{
    text.setFillColor(sf::Color::White);
    text.setPosition(CENTER_X - text.getGlobalBounds().width / 2.f, y);
}

sf::RectangleShape makeButton(float y, sf::Color color)
{
    sf::RectangleShape button({BUTTON_WIDTH, BUTTON_HEIGHT});

    button.setPosition(CENTER_X - BUTTON_WIDTH / 2.f, y);
    button.setFillColor(color);
    return button;
}

} // anonymous namespace

Menu::Menu() : _playButton(makeButton(PLAY_Y, sf::Color(60, 140, 70))), _quitButton(makeButton(QUIT_Y, sf::Color(150, 60, 60)))
{
    if (!_font.loadFromFile("assets/font.ttf")
        && !_font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf"))
        throw std::runtime_error("Menu: font not found (add a .ttf in assets/font.ttf)");
}

void Menu::draw(sf::RenderWindow &window)
{
    sf::Text title("ZAPPY", _font, 120);
    sf::Text playText("Play", _font, 40);
    sf::Text quitText("Quit", _font, 40);

    centerText(title, TITLE_Y);
    centerText(playText, PLAY_Y + 15.f);
    centerText(quitText, QUIT_Y + 15.f);
    window.draw(title);
    window.draw(_playButton);
    window.draw(_quitButton);
    window.draw(playText);
    window.draw(quitText);
}

} // namespace Zappy::Gui
