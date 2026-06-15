/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Player
*/

#ifndef ZAPPY_PLAYER_HPP_
#define ZAPPY_PLAYER_HPP_

#include <string_view>

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

    /**
     * @brief Cree un joueur a une position et direction aleatoires
     */
    [[nodiscard]] static Player spawnRandom(int width, int height);

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

    /**
     * @brief Execute une commande de deplacement, false si inconnue
     */
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
};

} // namespace Zappy::Server

#endif
