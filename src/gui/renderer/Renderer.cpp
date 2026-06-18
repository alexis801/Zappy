/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Renderer.cpp (GUI)
*/

#include "gui/Renderer.hpp"
#include "gui/Factory.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <iterator>
#include <stdexcept>

namespace Zappy::Gui {

namespace {

constexpr unsigned int WINDOW_WIDTH = 1920;
constexpr unsigned int WINDOW_HEIGHT = 1080;
constexpr float MARGIN = 80.f; // marge autour de la grille

// animation
constexpr float IDLE_FRAME_TIME = 0.15f;
constexpr float WALK_FRAME_TIME = 0.05f;
constexpr float MOVE_SPEED = 4.f; // fraction du chemin par seconde
constexpr float SRC_WIDTH = 682.f; // largeur real 1 frame robot
constexpr float ROBOT_SCALE = 1.2f; // robot par rapport a la largeur de case
constexpr float EGG_SCALE = 0.3f; //oeuf scale
constexpr float INCANT_SCALE = 0.9f; // cercle incantation
constexpr float ROBOT_FOOT = 0.70f; 

// dalles en iso

constexpr int CELL = 64; // taille cellule dans la sheet
constexpr int TILE_COL = 0;// colonne de la dalle sol
constexpr int TILE_ROW = 0; // ligne de la dalle sol
constexpr float TILE_Y_OFFSET = 0.f; // ajustement vertical

// materiaux (4 * 4, cel 64) index de la gemme

constexpr int MAT_CELL = 64;
constexpr float MATERIAL_SCALE = 0.3f;

const std::array<int, Zappy::RESOURCE_COUNT> RESOURCE_TILES = {{
    0, // FOOD -> vert
    10, // LINEMATE -> blanc/rose
    9, // DERAUMERE -> orange
    1, // SIBUR -> bleu
    2, // MENDIANE -> rouge
    3, // PHIRAS -> violet
    15, // THYSTAME -> cyan
}};

const std::array<sf::Color, Zappy::RESOURCE_COUNT> RESOURCE_COLORS = {{
    sf::Color(90, 200, 90),    // FOOD
    sf::Color(230, 230, 230),  // LINEMATE
    sf::Color(200, 120, 60),   // DERAUMERE
    sf::Color(80, 130, 230),   // SIBUR
    sf::Color(230, 220, 90),   // MENDIANE
    sf::Color(210, 100, 200),  // PHIRAS
    sf::Color(90, 220, 220),   // THYSTAME
}};

const std::array<sf::Color, 2> TEAM_COLORS = {{
    sf::Color(230, 80, 80),    // ROUGE
    sf::Color(80, 140, 230),  // BLEU
}};

// coleur d'equipe atribue par ordre arrive
sf::Color teamColor(const World &world, const std::string &team)
{
    const auto &teams = world.getTeams();

    for (std::size_t i = 0; i < teams.size(); ++i)
        if (teams[i] == team)
            return TEAM_COLORS[i % TEAM_COLORS.size()];
    return sf::Color::White;
}

float lengthSq(sf::Vector2f v)
{
    return v.x * v.x + v.y * v.y;
}

// calcul taille de case et origine pour centrer carte en isometrique
MapView computeView(int cols, int rows)
{
    MapView view;
    float availableW = static_cast<float>(WINDOW_WIDTH) - 2.f * MARGIN;
    float availableH = static_cast<float>(WINDOW_HEIGHT) - 2.f * MARGIN;
    float diagonal = static_cast<float>(cols + rows);

    view.tileW = std::min(2.f * availableW / diagonal, 4.f * availableH / diagonal);
    view.tileH = view.tileW / 2.f;
    view.originX = static_cast<float>(WINDOW_WIDTH) / 2.f - static_cast<float>(cols - rows) * view.tileW / 4.f;
    view.originY = static_cast<float>(WINDOW_HEIGHT) / 2.f - diagonal * view.tileH / 4.f;
    return view;
}

// sommet haut du losange de la case x y
sf::Vector2f tileTop(const MapView &view, int x, int y)
{
    return {
        view.originX + static_cast<float>(x - y) * view.tileW / 2.f,
        view.originY + static_cast<float>(x + y) * view.tileH / 2.f,
    };
}

// centre du losange de la case x y
sf::Vector2f tileCenter(const MapView &view, int x, int y)
{
    sf::Vector2f top = tileTop(view, x, y);

    return {top.x, top.y + view.tileH / 2.f};
}

} // anonymous namespace

Renderer::Renderer() : _window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Zappy GUI")
{
    _window.setFramerateLimit(60);
    if (!_robot.load())
        throw std::runtime_error("Renderer: robot sprites not found" "Check if assets folder has ressources");
    if (!_eggTexture.loadFromFile("assets/White_egg.png"))
        throw std::runtime_error("Renderer: egg sprite not found (assets/White_egg.png)");
    if (!_tilesTexture.loadFromFile("assets/tiles.png"))
        throw std::runtime_error("Renderer: tile sheet not found (assets/tiles.png)");
    _tilesTexture.setSmooth(false);
    if (!_materialsTexture.loadFromFile("assets/materials.png"))
        throw std::runtime_error("Renderer: materials sheet not found (assets/materials.png)");
    _materialsTexture.setSmooth(false);
    if (!_incantationTexture.loadFromFile("assets/incantation.png"))
        throw std::runtime_error("Renderer: incantation sheet not found (assets/materials.png)");
     _incantationTexture.setSmooth(false);
}

bool Renderer::isOpen() const
{
    return _window.isOpen();
}

bool Renderer::pollEvent(sf::Event &event)
{
    return _window.pollEvent(event);
}

void Renderer::close()
{
    _window.close();
}

void Renderer::drawMenu(Menu &menu)
{
    _window.clear(sf::Color(20, 20, 30));
    menu.draw(_window);
    _window.display();
}

// dalle iso en sprite dessiner de arriere a avant odre croissant
void Renderer::drawGrid(const World &world, const MapView &view)
{
    int w = world.getWidth();
    int h = world.getHeight();
    sf::Sprite sprite(_tilesTexture);
    float scale = view.tileW / static_cast<float>(CELL);

    sprite.setTextureRect(sf::IntRect(TILE_COL * CELL, TILE_ROW * CELL, CELL, CELL));
    sprite.setOrigin(static_cast<float>(CELL) / 2.f, 0.f);
    sprite.setScale(scale, scale);
    for (int sum = 0; sum <= (w - 1) + (h - 1); ++sum)
        for (int x = 0; x < w; ++x) {
            int y = sum - x;

            if (y < 0 || y >= h)
                continue;
            sf::Vector2f top = tileTop(view, x, y);

            sprite.setPosition(top.x, top.y + TILE_Y_OFFSET);
            _window.draw(sprite);
        }
}

// une ressource = point range mini grille 4 cols
void Renderer::drawResource(std::size_t resource, sf::Vector2f center, const MapView &view)
{
    int idx = RESOURCE_TILES[resource];
    float scale = (view.tileW * MATERIAL_SCALE) / static_cast<float>(MAT_CELL);
    float stepX = view.tileW / 4.5f;
    float stepY = view.tileH / 3.f;
    float col = static_cast<float>(resource % 4);
    float row = static_cast<float>(resource / 4);
    float gx = center.x + (col - 1.5f) * stepX;
    float gy = center.y + (row - 0.5f) * stepY - view.tileH * 0.1f;
    sf::Sprite sprite(_materialsTexture);

    sprite.setTextureRect(sf::IntRect((idx % 4) * MAT_CELL, (idx / 4) * MAT_CELL, MAT_CELL, MAT_CELL));
    sprite.setOrigin(static_cast<float>(MAT_CELL) / 2.f, static_cast<float>(MAT_CELL) / 2.f);
    sprite.setScale(scale, scale);
    sprite.setPosition(gx, gy);
    _window.draw(sprite);
}

// Les ressources qui sont sur une seule case
void Renderer::drawTileResources(const World::Tile &tile, sf::Vector2f center, const MapView &view)
{
    for (std::size_t i = 0; i < tile.size(); ++i)
        if (tile[i] > 0)
            drawResource(i, center, view);
}

void Renderer::drawResources(const World &world, const MapView &view)
{
    for (int y = 0; y < world.getHeight(); ++y)
        for (int x = 0; x < world.getWidth(); ++x)
            drawTileResources(world.getTile(x, y), tileCenter(view, x, y), view);
}

// ajouter ou retirer le viseul joueur selon le world
void Renderer::syncPlayers(const World &world, const MapView &view)
{
    const auto &players = world.getPlayers();

    for (auto it = _visuals.begin(); it != _visuals.end();)
        it = players.count(it->first) ? std::next(it) : _visuals.erase(it);

    for (const auto &[id, p] : players) {
        PlayerVisual &v = _visuals[id];

        v.target = tileCenter(view, p.x, p.y);
        v.orientation = p.orientation;
        v.color = teamColor(world, p.team);
        if (!v.initialized) {
            v.pos = v.target;
            v.initialized = true;
        }
    }
}

// interpol la position idle walk et avance frame
void Renderer::updateVisual(PlayerVisual &v, float dt)
{
    sf::Vector2f delta = v.target - v.pos;

    if (lengthSq(delta) > 1.f) {
        v.state = AnimState::Walk;
        v.pos += delta * std::min(1.f, MOVE_SPEED * dt);
    } else {
        v.pos = v.target;
        v.state = AnimState::Idle;
    }

    const Animation &anim = (v.state == AnimState::Walk) ? _robot.walk : _robot.idle;
    float frameTime = (v.state == AnimState::Walk) ? WALK_FRAME_TIME : IDLE_FRAME_TIME;

    if (anim.size() == 0)
        return;
    v.timer += dt;
    if (v.timer >= frameTime) {
        v.timer -= frameTime;
        v.frame = (v.frame + 1) % anim.size();
    }
    if (v.frame >= anim.size())
        v.frame = 0;
}

// set origine au centre pied du robot, origin = / nord 1 / est 2 sud 3 ouest 4 + pied au centre de la case
void Renderer::drawRobot(const PlayerVisual &v, const MapView &view)
{
    const Animation &anim = (v.state == AnimState::Walk) ? _robot.walk : _robot.idle;

    if (anim.size() == 0)
        return;
    sf::Sprite sprite(anim.frameAt(v.frame));
    auto bounds = sprite.getLocalBounds();

    sprite.setColor(v.color);

    sprite.setOrigin(bounds.width / 2.f, bounds.height * ROBOT_FOOT);

    float scale = (view.tileW * ROBOT_SCALE) / SRC_WIDTH;

    bool faceLeft = (v.orientation == 1 || v.orientation == 4);
    sprite.setScale(faceLeft ? -scale : scale, scale);

    sprite.setPosition(v.pos.x, v.pos.y);
    _window.draw(sprite);
}

void Renderer::drawPlayers(const World &world, const MapView &view, float dt)
{
    syncPlayers(world, view);
    for (auto &[id, v] : _visuals) {
        (void)id;
        updateVisual(v, dt);
        drawRobot(v, view);
    }
}

// oeuf centre sur la tile comme le player
void Renderer::drawEggs(const World &world, const MapView &view)
{
    sf::Vector2u size = _eggTexture.getSize();

    if (size.x == 0)
        return;
    float scale = (view.tileW * EGG_SCALE) / static_cast<float>(size.x);

    for (const auto &entry : world.getEggs()) {
        const Egg &egg = entry.second;
        sf::Vector2f center = tileCenter(view, egg.x, egg.y);
        sf::Sprite sprite(_eggTexture);

        sprite.setOrigin(static_cast<float>(size.x) / 2.f, static_cast<float>(size.y));
        sprite.setScale(scale, scale);
        sprite.setPosition(center.x, center.y);
        _window.draw(sprite);
    }
}

void Renderer::drawIncantations(const World &world, const MapView &view)
{
    sf::Vector2u size = _incantationTexture.getSize();

    if (size.x == 0)
        return;
    float scale = (view.tileW * INCANT_SCALE) / static_cast<float>(size.x);

    for (const auto &cell : world.getIncantations()) {
        sf::Vector2f center = tileCenter(view, cell.first, cell.second);
        sf::Sprite sprite(_incantationTexture);

        sprite.setOrigin(static_cast<float>(size.x) / 2.f, static_cast<float>(size.y) / 2.f);
        sprite.setScale(scale, scale);
        sprite.setPosition(center.x, center.y);
        _window.draw(sprite);
    }
}

void Renderer::draw(const World &world)
{
    float dt = _frameClock.restart().asSeconds();

    _window.clear(sf::Color(15, 12, 25));
    if (world.getWidth() > 0 && world.getHeight() > 0) {
        MapView view = computeView(world.getWidth(), world.getHeight());

        drawGrid(world, view);
        drawResources(world, view);
        drawIncantations(world, view);
        drawEggs(world, view);
        drawPlayers(world, view, dt);
    }
    _window.display();
}

} // namespace Zappy::Gui