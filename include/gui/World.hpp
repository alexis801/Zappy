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
#include <utility>

namespace Zappy::Gui {

struct Player {
    int id = 0;
    int x = 0;
    int y = 0;
    int orientation = 1; // 1= nord, 2= est, 3= sdu, 4= ouest
    int level = 1;
    std::string team;
    std::array<int, Zappy::RESOURCE_COUNT> inventory{};
};

struct Egg {
    int id = 0;
    int playerId = 0;
    int x = 0;
    int y = 0;
};

struct GameEvent {
    int x = 0;
    int y = 0;
    std::string txt;
};

/**
 * @class World
 * @brief Copie locale du monde taille de la carte + cases (ressources)
 */

class World {
public:

    // Une case = 7 quantités de ressources (indsex par resources)
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

    [[nodiscard]] const std::vector<std::string> &getTeams() const noexcept
    {
        return _teams;
    }

// eggs methods

    void addEgg(const Egg &egg) { _eggs[egg.id] = egg; }
    void removeEgg(int id) { _eggs.erase(id); }

    [[nodiscard]] const std::unordered_map<int, Egg> &getEggs() const noexcept
    {
        return _eggs;
    }

// player inventory pin

    void setPlayerInventory(int id, int x, int y, const std::array<int, Zappy::RESOURCE_COUNT> &inventory)
    {
        auto it = _players.find(id);
        if (it == _players.end())
            return;
        it->second.x = x;
        it->second.y = y;
        it->second.inventory = inventory;
    }

// player incantations methods

    void startIncantation(int x, int y)
    {
        for (const auto &cell : _incantations)
            if (cell.first == x && cell.second == y)
                return;
        _incantations.emplace_back(x, y);
    }

    void endIncantation(int x, int y)
    {
        for (auto i = _incantations.begin(); i != _incantations.end(); i++)
            if (i->first == x && i->second == y) {
                _incantations.erase(i);
                return;
        }
    }

    [[nodiscard]] const std::vector<std::pair<int, int>> &getIncantations() const noexcept
    {
        return _incantations;
    }

// party state methods (sgt sst seg smg)

    void setTimeUnit(int unit) { _timeUnit = unit; }
    [[nodiscard]] int getTimeUnit() const noexcept { return _timeUnit; }

    void endGame(const std::string &winner)
    {
        _gameOver = true;
        _winner = winner;
    }

    [[nodiscard]] bool isGameOver() const noexcept { return _gameOver; }
    [[nodiscard]] const std::string &getWinner() const noexcept { return _winner; }

    void setServerMessage(const std::string &message) { _serverMessage = message; }
    [[nodiscard]] const std::string &getServerMessage() const noexcept
    {
        return _serverMessage;
    }

// event ponctuel ratache a la case du joeur pbc/pex/pdr/pgt/pfk

void pushPlayerEvent(int id, const std::string &txt)
{
    auto it = _players.find(id);
    if (it != _players.end())
        _events.push_back({it->second.x, it->second.y, txt});
}

[[nodiscard]] std::vector<GameEvent> takeEvents()
{
    std::vector<GameEvent> out = std::move(_events);
    _events.clear();
    return out;
}

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
    std::vector<GameEvent> _events;
    std::vector<std::string> _teams;
    std::unordered_map<int, Egg> _eggs;
    std::vector<std::pair<int, int>> _incantations;
    int _timeUnit = 0;
    bool _gameOver = false;
    std::string _winner;
    std::string _serverMessage;
};

} // namespace Zappy::Gui

#endif // ZAPPY_GUI_WORLD_HPP_