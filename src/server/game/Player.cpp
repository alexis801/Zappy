/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Player
*/

#include "Player.hpp"

namespace Zappy::Server {

Player::Player(int x, int y, Direction direction) noexcept
    : _x(x), _y(y), _direction(direction)
{
}

int Player::getX() const noexcept
{
    return _x;
}

int Player::getY() const noexcept
{
    return _y;
}

Direction Player::getDirection() const noexcept
{
    return _direction;
}

} // namespace Zappy::Server
