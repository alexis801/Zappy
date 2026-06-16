/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Menu (GUI)
*/

#include "gui/Menu.hpp"
#include "gui/Factory.hpp"

#include <stdexcept>
#include <string>

namespace Zappy::Gui {

namespace {

constexpr float WINDOW_WIDTH = 1920.f;
constexpr float WINDOW_HEIGHT = 1080.f;
constexpr float BUTTON_LEFT = 100.f;
constexpr float FIRST_BUTTON_Y = 340.f;
constexpr float BUTTON_SPACING = 140.f;


const std::array<std::string, 3> FRAMES = {
    "assets/button_none.png",
    "assets/button_hover.png",
    "assets/button_clicked.png",
};

const std::array<std::string, 3> LABELS = {"PLAY", "OPTIONS", "QUIT"};
const std::array<Menu::ButtonAction, 3> ACTIONS = {Menu::ButtonAction::Play, Menu::ButtonAction::Options, Menu::ButtonAction::Quit};

sf::Vector2f buttonPos(std::size_t index)
{
    return {BUTTON_LEFT, FIRST_BUTTON_Y + static_cast<float>(index) * BUTTON_SPACING};
}


} // anonymous namespace

Menu::Menu()
{
    if (!_font.loadFromFile("assets/font.ttf") && !_font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf"))
        throw std::runtime_error("Menu: font not found (add a .ttf in assets/font.ttf)");
    if (_backgroundTexture.loadFromFile("assets/menu_bg_final.png")) {
        sf::Vector2u size = _backgroundTexture.getSize();

        _background.setTexture(_backgroundTexture);
        _background.setScale(WINDOW_WIDTH / static_cast<float>(size.x), WINDOW_HEIGHT / static_cast<float>(size.y));
    }
    for (std::size_t i = 0; i < _buttons.size(); ++i)
        if (!_buttons[i].load(FRAMES, _font, LABELS[i], buttonPos(i)))
            throw std::runtime_error(
                "Menu: button sprite not found (assets/button_*.png)");
}

void Menu::updateHover(sf::Vector2f mouse)
{
    for (Button &button : _buttons)
        if (button.getState() != Button::State::Clicked)
            button.setState(button.contains(mouse) ? Button::State::Hover : Button::State::None);
}

Menu::ButtonAction Menu::release(sf::Vector2f mouse)
{
    ButtonAction result = ButtonAction::None;

    for (std::size_t i = 0; i < _buttons.size(); ++i) {
        if (_buttons[i].getState() != Button::State::Clicked)
            continue;
        if (_buttons[i].contains(mouse)) {
            result = ACTIONS[i];
            _buttons[i].setState(Button::State::Hover);
        } else {
            _buttons[i].setState(Button::State::None);
        }
    }
    return result;
}

void Menu::press(sf::Vector2f mouse)
{
    for (Button &button : _buttons)
        if (button.contains(mouse))
            button.setState(Button::State::Clicked);
}

void Menu::draw(sf::RenderWindow &window)
{
    window.draw(_background);
    for (const Button &button : _buttons)
        button.draw(window);
}

} // namespace Zappy::Gui
