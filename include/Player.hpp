/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Player
*/

#ifndef ZAPPY_PLAYER_HPP_
#define ZAPPY_PLAYER_HPP_

namespace Zappy::Server {

/**
 * @enum Direction
 * @brief Les 4 directions possibles d'un joueur
 */
enum class Direction {
    NORTH,
    EAST,
    SOUTH,
    WEST,
};

/**
 * @class Player
 * @brief Un joueur sur la carte : position et direction
 */
class Player {
public:
    Player(int x, int y, Direction direction) noexcept;

    [[nodiscard]] int getX() const noexcept;
    [[nodiscard]] int getY() const noexcept;
    [[nodiscard]] Direction getDirection() const noexcept;

private:
    int _x;
    int _y;
    Direction _direction;
};

} // namespace Zappy::Server

#endif
