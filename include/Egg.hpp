/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Egg
*/

#ifndef ZAPPY_EGG_HPP_
#define ZAPPY_EGG_HPP_

#include <optional>
#include <random>
#include <string>
#include <string_view>
#include <vector>

namespace Zappy::Server {

class Egg {
public:
    Egg(int id, std::string team, int x, int y) noexcept;

    [[nodiscard]] int getId() const noexcept;
    [[nodiscard]] const std::string &getTeam() const noexcept;
    [[nodiscard]] int getX() const noexcept;
    [[nodiscard]] int getY() const noexcept;

    [[nodiscard]] static std::optional<Egg> pickRandomForTeam(
        std::vector<Egg> &eggs, std::string_view teamName,
        std::mt19937 &rng) noexcept;

    static void destroyOnTile(
        std::vector<Egg> &eggs, int x, int y) noexcept;

private:
    int _id;
    std::string _team;
    int _x;
    int _y;
};

} // namespace Zappy::Server

#endif
