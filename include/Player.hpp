/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Player
*/

#ifndef ZAPPY_PLAYER_HPP_
#define ZAPPY_PLAYER_HPP_

#include "Macros.hpp"
#include <string_view>

namespace Zappy::Server {

enum class Direction {
    NORTH,
    EAST,
    SOUTH,
    WEST,
};

class Player {
public:
    Player(int x, int y, Direction direction) noexcept;

    [[nodiscard]] static Player spawnRandom(int width, int height);

    [[nodiscard]] int getX() const noexcept;
    [[nodiscard]] int getY() const noexcept;
    [[nodiscard]] Direction getDirection() const noexcept;
    [[nodiscard]] int getLevel() const noexcept;
    [[nodiscard]] int getFood() const noexcept;

    void forward(int width, int height) noexcept;
    void turnLeft() noexcept;
    void turnRight() noexcept;

    [[nodiscard]] bool executeMovement(
        std::string_view command, int width, int height) noexcept;

private:
    static constexpr int DIRECTION_COUNT = 4;

    static int drawRandomInt(int upperExclusive);

    void stepNorth(int height) noexcept;
    void stepEast(int width) noexcept;
    void stepSouth(int height) noexcept;
    void stepWest(int width) noexcept;

    int _x;
    int _y;
    Direction _direction;
    int _level;
    int _food;
};

} // namespace Zappy::Server

#endif
