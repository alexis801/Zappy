/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** World.hpp (GUI)
*/

#ifndef ZAPPY_GUI_WORLD_HPP_
#define ZAPPY_GUI_WORLD_HPP_

#include "Macros.hpp"

#include <array>
#include <vector>
#include <string>
#include <unordered_map>

namespace Zappy::Gui {

struct Player {
    int id = 0;
    int x = 0;
    int y = 0;
    int orientation = 1; // 1= nord, 2= est, 3= sdu, 4= ouest
    int level = 1;
    std::string team;
};

/**
 * @class World
 * @brief Copie locale du monde taille de la carte + cases (ressources)
 */

class World {
public:

    // Une case = 7 quantités de ressources (index par resources)
    using Tile = std::array<int, Zappy::RESOURCE_COUNT>;

    [[nodiscard]] int getWidth() const noexcept { return _width; }
    [[nodiscard]] int getHeight() const noexcept { return _height; }

    // Fixe la taille et alloue/ re alloue les case a 0
    void setSize(int width, int height)
    {
        _width = width;
        _height = height;
        _tiles.assign(static_cast<std::size_t>(width) * height, Tile{});
    }

    // Met a jour le contenue d'une case (ignore si hors)
    void setTile(int x, int y, const Tile &tile)
    {
        if (contains(x, y))
            _tiles[index(x, y)] = tile;
    }

    [[nodiscard]] const Tile &getTile(int x, int y) const
    {
        return _tiles[index(x, y)];
    }

// player methods

    void addPlayer(const Player &player) { _players[player.id] = player; }

    void movePlayer(int id, int x, int y, int orientation)
    {
        auto it = _players.find(id);
        if (it == _players.end())
            return;
        it->second.x = x;
        it->second.y = y;
        it->second.orientation = orientation;
    }

    void setPlayerLevel(int id, int level)
    {
        auto it = _players.find(id);
        if (it != _players.end())
            it->second.level = level;
    }

    void removePlayer(int id) { _players.erase(id); }

    [[nodiscard]] const std::unordered_map<int, Player> &getPlayers() const noexcept
    {
        return _players;
    }

// teams methods

    void addTeam(const std::string &name)
    {
        for (const std::string &team : _teams)
            if (team == name)
                return;
        _teams.push_back(name);
    }

    [[nodiscard]] const std::vector<std::string> &getTeams() const noexcept;

private:

    [[nodiscard]] bool contains(int x, int y) const noexcept
    {
        return x >= 0 && y >= 0 && x < _width && y < _height;
    }

        [[nodiscard]] std::size_t index(int x, int y) const noexcept
    {
        return static_cast<std::size_t>(y) * _width + x;
    }

    int _width = 0;
    int _height = 0;
    std::vector<Tile> _tiles;
    std::unordered_map<int, Player> _players;
    std::vector<std::string> _teams;
};

} // namespace Zappy::Gui

#endif // ZAPPY_GUI_WORLD_HPP_