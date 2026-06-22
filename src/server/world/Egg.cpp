/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Egg
*/

#include "Egg.hpp"

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

} // namespace Zappy::Server
