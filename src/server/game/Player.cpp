/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Player
*/

#include "Egg.hpp"
#include "Player.hpp"
#include "Team.hpp"
#include <random>
#include <string>

namespace Zappy::Server {

int Player::drawRandomInt(int upperExclusive)
{
    static std::mt19937 generator{std::random_device{}()};
    std::uniform_int_distribution<int> distribution(0, upperExclusive - 1);

    return distribution(generator);
}

Player::Player(int x, int y, Direction direction, int id) noexcept
    : _x(x), _y(y), _direction(direction), _id(id), _level(1), _pendingTime(0)
{
    _inventory[static_cast<std::size_t>(Resource::FOOD)] = LIFE_UNITS_AT_START;
}

int Player::getId() const noexcept
{
    return _id;
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

int Player::getLevel() const noexcept
{
    return _level;
}

void Player::setLevel(int level) noexcept
{
    _level = level;
}

bool Player::isFrozen() const noexcept
{
    return _frozen;
}

void Player::setFrozen(bool frozen) noexcept
{
    _frozen = frozen;
}

int Player::getFood() const noexcept
{
    return _inventory[static_cast<std::size_t>(Resource::FOOD)];
}

int Player::get(Resource resource) const noexcept
{
    return _inventory[static_cast<std::size_t>(resource)];
}

std::string Player::formatInventory() const
{
    std::string result = "[";

    for (std::size_t i = 0; i < RESOURCE_COUNT; ++i) {
        if (i > 0)
            result += ", ";
        result += RESOURCE_NAMES[i];
        result += ' ';
        result += std::to_string(_inventory[i]);
    }
    result += ']';
    return result;
}

void Player::consumeFood(int elapsedTimeUnits) noexcept
{
    if (!isAlive())
        return;
    _pendingTime += elapsedTimeUnits;
    std::size_t foodIndex = static_cast<std::size_t>(Resource::FOOD);

    while (_pendingTime >= TIME_UNITS_PER_FOOD && _inventory[foodIndex] > 0) {
        _pendingTime -= TIME_UNITS_PER_FOOD;
        --_inventory[foodIndex];
    }
    if (_inventory[foodIndex] <= 0)
        _pendingTime = 0;
}

bool Player::isAlive() const noexcept
{
    return getFood() > 0;
}

bool Player::eat(Tile &tile) noexcept
{
    std::size_t foodIndex = static_cast<std::size_t>(Resource::FOOD);

    if (tile.get(Resource::FOOD) <= 0)
        return false;
    tile.remove(Resource::FOOD, 1);
    ++_inventory[foodIndex];
    return true;
}

bool Player::fork(
    Team &team, std::vector<Egg> &eggs, int &nextEggId) noexcept
{
    eggs.emplace_back(nextEggId++, team.getName(), _x, _y);
    team.addSlot();
    return true;
}

Player Player::spawnRandom(int width, int height, int id)
{
    return Player(drawRandomInt(width), drawRandomInt(height),
        static_cast<Direction>(drawRandomInt(DIRECTION_COUNT)), id);
}

Player Player::spawnFromEgg(const Egg &egg, int id)
{
    return Player(egg.getX(), egg.getY(),
        static_cast<Direction>(drawRandomInt(DIRECTION_COUNT)), id);
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

bool Player::runMovement(
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

CommandOutcome Player::executeCommand(
    std::string_view command, int width, int height) noexcept
{
    if (command == "Inventory")
        return {CommandKind::INVENTORY, formatInventory()};
    if (runMovement(command, width, height))
        return {CommandKind::OK, {}};
    return {CommandKind::KO, {}};
}

std::string Player::directionToString()
{
    switch (_direction) {
        case Direction::NORTH: return "NORTH";
        case Direction::EAST:  return "EAST";
        case Direction::SOUTH: return "SOUTH";
        case Direction::WEST:  return "WEST";
    }
    return "UNKNOWN";
}

void Player::addResource(Resource res) noexcept
{
    ++_inventory[static_cast<std::size_t>(res)];
}

bool Player::removeResource(Resource res) noexcept
{
    std::size_t index = static_cast<std::size_t>(res);

    if (_inventory[index] <= 0)
        return false;
    --_inventory[index];
    return true;
}

} // namespace Zappy::Server
