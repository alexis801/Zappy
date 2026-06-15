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

const std::array<sf::Color, Zappy::RESOURCE_COUNT> RESOURCE_COLORS = {{
    sf::Color(90, 200, 90),    // FOOD
    sf::Color(230, 230, 230),  // LINEMATE
    sf::Color(200, 120, 60),   // DERAUMERE
    sf::Color(80, 130, 230),   // SIBUR
    sf::Color(230, 220, 90),   // MENDIANE
    sf::Color(210, 100, 200),  // PHIRAS
    sf::Color(90, 220, 220),   // THYSTAME
}};

float cellSize(int cols, int rows)
{
    return std::min(static_cast<float>(WINDOW_WIDTH) / cols, static_cast<float>(WINDOW_HEIGHT) / rows);
}

} // anonymous namespace

Renderer::Renderer() : _window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Zappy GUI")
{
    _window.setFramerateLimit(60);
}

bool Renderer::isOpen() const
{
    return _window.isOpen();
}

void Renderer::handleEvents()
{
    sf::Event event;

    while (_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            _window.close();
    }
}

void Renderer::drawMenu(Menu &menu)
{
    _window.clear(sf::Color(20, 20, 30));
    menu.draw(_window);
    _window.display();
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

// Un point represente une ressource dans une mini grille de 4 cols
void Renderer::drawResourceDot(std::size_t resource, int x, int y, float cell)
{
    float dot = cell / 9.f;
    sf::CircleShape shape(dot);
    float px = x * cell + static_cast<float>(resource % 4) * (dot * 2.2f) + dot;
    float py = y * cell + static_cast<float>(resource / 4) * (dot * 2.2f) + dot;

    shape.setPosition(px, py);
    shape.setFillColor(RESOURCE_COLORS[resource]);
    _window.draw(shape);
}

// Les ressources qui sont sur une seule case
void Renderer::drawTileResources(const World::Tile &tile, int x, int y, float cell)
{
    for (std::size_t i = 0; i < tile.size(); ++i) {
        if (tile[i] > 0)
            drawResourceDot(i, x, y, cell);
    }
}

// Toues les cases de la map 
void Renderer::drawResources(const World &world)
{
    int cols = world.getWidth();
    int rows = world.getHeight();

    if (cols <= 0 || rows <= 0)
        return;
    float cell = cellSize(cols, rows);
    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x)
            drawTileResources(world.getTile(x, y), x, y, cell);
    }
}

void Renderer::draw(const World &world)
{
    _window.clear(sf::Color(20, 20, 30));
    drawGrid(world);
    drawResources(world);
    _window.display();
}

} // namespace Zappy::Gui