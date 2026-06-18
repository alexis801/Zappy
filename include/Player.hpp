/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Player
*/

#ifndef ZAPPY_PLAYER_HPP_
#define ZAPPY_PLAYER_HPP_

#include "Macros.hpp"
#include <array>
#include <string>
#include <string_view>

namespace Zappy::Server {

enum class Direction {
    NORTH,
    EAST,
    SOUTH,
    WEST,
};

enum class CommandKind {
    KO,
    OK,
    INVENTORY,
};

struct CommandOutcome {
    CommandKind kind = CommandKind::KO;
    std::string text;
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
    [[nodiscard]] int get(Resource resource) const noexcept;

    [[nodiscard]] std::string formatInventory() const;

    void forward(int width, int height) noexcept;
    void turnLeft() noexcept;
    void turnRight() noexcept;

    [[nodiscard]] CommandOutcome executeCommand(
        std::string_view command, int width, int height) noexcept;

private:
    static constexpr int DIRECTION_COUNT = 4;

    static int drawRandomInt(int upperExclusive);

    [[nodiscard]] bool runMovement(
        std::string_view command, int width, int height) noexcept;

    void stepNorth(int height) noexcept;
    void stepEast(int width) noexcept;
    void stepSouth(int height) noexcept;
    void stepWest(int width) noexcept;

    int _x;
    int _y;
    Direction _direction;
    int _level;
    std::array<int, RESOURCE_COUNT> _inventory{};
};

} // namespace Zappy::Server

#endif
