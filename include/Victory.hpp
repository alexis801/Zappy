/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Victory
*/

#ifndef ZAPPY_VICTORY_HPP_
#define ZAPPY_VICTORY_HPP_

#include <string_view>
#include <vector>

namespace Zappy::Server {

struct PlayerSnapshot {
    std::string_view team;
    int level;
    bool alive;
};

/**
 * @class Victory
 * @brief Compte les joueurs au niveau max par equipe
 */
class Victory {
public:
    [[nodiscard]] static int countLevel8ForTeam(
        const std::vector<PlayerSnapshot> &players,
        std::string_view team) noexcept;
};

} // namespace Zappy::Server

#endif
