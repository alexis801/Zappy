/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Tile
*/

#include "Tile.hpp"
#include <algorithm>

namespace Zappy::Server {

int Tile::get(Resource resource) const noexcept
{
    return _quantities[static_cast<std::size_t>(resource)];
}

void Tile::add(Resource resource, int amount) noexcept
{
    _quantities[static_cast<std::size_t>(resource)] += amount;
}

void Tile::remove(Resource resource, int amount) noexcept
{
    std::size_t index = static_cast<std::size_t>(resource);
    int remaining = _quantities[index] - amount;

    _quantities[index] = remaining > 0 ? remaining : 0;
}

bool Tile::isEmpty() const noexcept
{
    for (int quantity : _quantities) {
        if (quantity > 0)
            return false;
    }
    return true;
}

int Tile::total() const noexcept
{
    int sum = 0;

    for (int quantity : _quantities)
        sum += quantity;
    return sum;
}

void Tile::addPlayer(int playerId) noexcept
{
    _players.push_back(playerId);
}

void Tile::removePlayer(int playerId) noexcept
{
    auto it = std::find(_players.begin(), _players.end(), playerId);

    if (it != _players.end())
        _players.erase(it);
}

const std::vector<int> &Tile::players() const noexcept
{
    return _players;
}

int Tile::resourceCount(Resource resource) const noexcept
{
    return get(resource);
}

} // namespace Zappy::Server
