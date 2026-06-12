/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Renderer.cpp (GUI)
*/

#include "gui/Renderer.hpp"

#include <algorithm>

namespace Zappy::Gui {

namespace {

constexpr unsigned int WINDOW_WIDTH = 1920;
constexpr unsigned int WINDOW_HEIGHT = 1080;

} // anonymous namespace

Renderer::Renderer() : _window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Zappy GUI")
{
    _window.setFramerateLimit(60);
}

bool Renderer::isOpen() const
{
    return _window.isOpen();
}

void Renderer::handleEvent()
{
    sf::Event event;

    while (_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            _window.close();
    }
}

void Renderer::drawGrid(const World &world)
{
    int cols = world.getWidth();
    int rows = world.getHeight();

    if (cols <= 0 || rows <= 0)
        return;
    float cell = std::min(static_cast<float>(WINDOW_WIDTH) / cols,
        static_cast<float>(WINDOW_HEIGHT) / rows);
    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            sf::RectangleShape tile({cell - 1.f, cell - 1.f});

            tile.setPosition(x * cell, y * cell);
            tile.setFillColor(sf::Color(40, 80, 50));
            _window.draw(tile);
        }
    }
}

void Renderer::draw(const World &world)
{
    _window.clear(sf::Color(20, 20, 30));
    drawGrid(world);
    _window.display();
}

} // namespace Zappy::Gui