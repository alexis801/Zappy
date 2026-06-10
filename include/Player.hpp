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

    /**
     * @brief Avance d'une case dans sa direction (monde toroidal)
     */
    void forward(int width, int height) noexcept;

    /**
     * @brief Tourne de 90 degres a gauche
     */
    void turnLeft() noexcept;

    /**
     * @brief Tourne de 90 degres a droite
     */
    void turnRight() noexcept;

private:
    static constexpr int DIRECTION_COUNT = 4;

    void stepNorth(int height) noexcept;
    void stepEast(int width) noexcept;
    void stepSouth(int height) noexcept;
    void stepWest(int width) noexcept;

    int _x;
    int _y;
    Direction _direction;
};

} // namespace Zappy::Server

#endif
