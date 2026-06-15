/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Menu (GUI)
*/

#include "gui/Menu.hpp"
#include "gui/Factory.hpp"

#include <stdexcept>

namespace Zappy::Gui {

namespace {

constexpr float WINDOW_WIDTH = 1920.f;
constexpr float WINDOW_HEIGHT = 1080.f;
constexpr float CENTER_X = WINDOW_WIDTH / 2.f;
constexpr float TITLE_Y = 200.f;
constexpr float BUTTON_WIDTH = 300.f;
constexpr float BUTTON_HEIGHT = 80.f;
constexpr float PLAY_Y = 450.f;
constexpr float QUIT_Y = 580.f;
constexpr float LABEL_OFFSET = 15.f;

sf::Vector2f buttonPos(float y)
{
    return {CENTER_X - BUTTON_WIDTH / 2.f, y};
}

} // anonymous namespace

Menu::Menu() : _playButton(factory::rect({BUTTON_WIDTH, BUTTON_HEIGHT}, buttonPos(PLAY_Y), sf::Color(60, 140, 70))), 
    _quitButton(factory::rect({BUTTON_WIDTH, BUTTON_HEIGHT}, buttonPos(QUIT_Y), sf::Color(150, 60, 60)))
{
    if (!_font.loadFromFile("assets/font.ttf")
        && !_font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf"))
        throw std::runtime_error("Menu: font not found (add a .ttf in assets/font.ttf)");
    
    if (_backgroundTexture.loadFromFile("assets/menu_bg.png")) {
        sf::Vector2u size = _backgroundTexture.getSize();

         _background.setTexture(_backgroundTexture);
        _background.setScale(WINDOW_WIDTH / static_cast<float>(size.x), WINDOW_HEIGHT / static_cast<float>(size.y));
    }
}

Menu::ButtonAction Menu::handleClick(sf::Vector2f position) const
{
    if (_playButton.getGlobalBounds().contains(position))
        return ButtonAction::Play;
    if (_quitButton.getGlobalBounds().contains(position))
        return ButtonAction::Quit;
    return ButtonAction::None;
}

void Menu::draw(sf::RenderWindow &window)
{
    sf::Text title = factory::text(_font, "ZAPPY", 120);
    sf::Text playText = factory::text(_font, "Play", 40);
    sf::Text quitText = factory::text(_font, "Quit", 40);

    factory::centerText(title, CENTER_X, TITLE_Y);
    factory::centerText(playText, CENTER_X, PLAY_Y + LABEL_OFFSET);
    factory::centerText(quitText, CENTER_X, QUIT_Y + LABEL_OFFSET);
    window.draw(_background);
    window.draw(_playButton);
    window.draw(_quitButton);
    window.draw(title);
    window.draw(playText);
    window.draw(quitText);
}

} // namespace Zappy::Gui
