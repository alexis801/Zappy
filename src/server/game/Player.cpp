/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Player
*/

#include "Player.hpp"
#include <random>

namespace Zappy::Server {

int Player::drawRandomInt(int upperExclusive)
{
    static std::mt19937 generator{std::random_device{}()};
    std::uniform_int_distribution<int> distribution(0, upperExclusive - 1);

    return distribution(generator);
}

Player::Player(int x, int y, Direction direction) noexcept
    : _x(x), _y(y), _direction(direction), _level(1),
      _food(LIFE_UNITS_AT_START)
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

Player Player::spawnRandom(int width, int height)
{
    return Player(drawRandomInt(width), drawRandomInt(height),
        static_cast<Direction>(drawRandomInt(DIRECTION_COUNT)));
}

void Player::stepNorth(int height) noexcept
{
    _y = (_y - 1 + height) % height;
}

void Player::stepEast(int width) noexcept
{
    _x = (_x + 1) % width;
}

void Player::stepSouth(int height) noexcept
{
    _y = (_y + 1) % height;
}

void Player::stepWest(int width) noexcept
{
    _x = (_x - 1 + width) % width;
}

void Player::forward(int width, int height) noexcept
{
    if (_direction == Direction::NORTH)
        stepNorth(height);
    else if (_direction == Direction::EAST)
        stepEast(width);
    else if (_direction == Direction::SOUTH)
        stepSouth(height);
    else
        stepWest(width);
}

void Player::turnLeft() noexcept
{
    _direction = static_cast<Direction>(
        (static_cast<int>(_direction) + 3) % DIRECTION_COUNT);
}

void Player::turnRight() noexcept
{
    _direction = static_cast<Direction>(
        (static_cast<int>(_direction) + 1) % DIRECTION_COUNT);
}

bool Player::executeMovement(
    std::string_view command, int width, int height) noexcept
{
    if (command == "Forward") {
        forward(width, height);
        return true;
    }
    if (command == "Left") {
        turnLeft();
        return true;
    }
    if (command == "Right") {
        turnRight();
        return true;
    }
    return false;
}

} // namespace Zappy::Server
