/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Egg
*/

#include "Egg.hpp"
#include <algorithm>
#include <utility>

namespace Zappy::Server {

Egg::Egg(int id, std::string team, int x, int y) noexcept
    : _id(id), _team(std::move(team)), _x(x), _y(y)
{
}

int Egg::getId() const noexcept
{
    return _id;
}

const std::string &Egg::getTeam() const noexcept
{
    return _team;
}

int Egg::getX() const noexcept
{
    return _x;
}

int Egg::getY() const noexcept
{
    return _y;
}

std::optional<Egg> Egg::pickRandomForTeam(
    std::vector<Egg> &eggs, std::string_view teamName,
    std::mt19937 &rng) noexcept
{
    std::vector<std::size_t> candidates;

    for (std::size_t i = 0; i < eggs.size(); ++i) {
        if (eggs[i].getTeam() == teamName)
            candidates.push_back(i);
    }
    if (candidates.empty())
        return std::nullopt;
    std::uniform_int_distribution<std::size_t> dist(0, candidates.size() - 1);
    std::size_t index = candidates[dist(rng)];

    Egg picked = eggs[index];
    eggs.erase(eggs.begin() + static_cast<std::ptrdiff_t>(index));
    return picked;
}

std::vector<int> Egg::destroyOnTile(
    std::vector<Egg> &eggs, int x, int y) noexcept
{
    std::vector<int> destroyed;

    auto newEnd = std::remove_if(eggs.begin(), eggs.end(),
        [&](const Egg &egg) {
            if (egg.getX() == x && egg.getY() == y) {
                destroyed.push_back(egg.getId());
                return true;
            }
            return false;
        });

    eggs.erase(newEnd, eggs.end());
    return destroyed;
}

} // namespace Zappy::Server
