/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Renderer.hpp (GUI)
*/

#ifndef ZAPPY_GUI_RENDERER_HPP_
#define ZAPPY_GUI_RENDERER_HPP_

#include "gui/World.hpp"
#include "gui/Menu.hpp"
#include "gui/PlayerVisual.hpp"

#include <cstddef>
#include <SFML/Graphics.hpp>
#include <unordered_map>

namespace Zappy::Gui {

// placement de la grille dans la fenetre : taille une case + coin haut gauche
struct MapView {

    float tileW = 0.f;
    float tileH = 0.f;
    float originX = 0.f;
    float originY = 0.f;
};

/**
 * @class Renderer
 * @brief Fenetre SFML draw le world
 */
class Renderer {
public:
    Renderer();

    [[nodiscard]] bool isOpen() const;
    bool pollEvent(sf::Event &event);
    void close();
    void draw(const World &world);
    void drawMenu(Menu &menu);


private:
    void drawGrid(const World &world, const MapView &view);
    void drawResources(const World &world, const MapView &view);
    void drawTileResources(const World::Tile &tile, sf::Vector2f center, const MapView &view);
    void drawResourceDot(std::size_t resource, sf::Vector2f center, const MapView &view);
    void drawPlayers(const World &world, const MapView &view, float dt);
    void syncPlayers(const World &world, const MapView &view);
    void updateVisual(PlayerVisual &v, float dt);
    void drawRobot(const PlayerVisual &v, const MapView &view);
    void drawEggs(const World &world, const MapView &view);

    sf::RenderWindow _window;
    RobotAssets _robot;
    sf::Texture _eggTexture;
    std::unordered_map<int, PlayerVisual> _visuals;
    sf::Clock _frameClock;
};

} // namespace Zappy::Gui

#endif // ZAPPY_GUI_RENDERER_HPP_