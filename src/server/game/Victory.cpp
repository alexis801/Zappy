/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Victory
*/

#include "Victory.hpp"
#include "Macros.hpp"

namespace Zappy::Server {

namespace {

[[nodiscard]] bool countsTowardVictory(
    const PlayerSnapshot &player, std::string_view team) noexcept
{
    return player.team == team && player.alive
        && player.level >= Zappy::MAX_PLAYER_LEVEL;
}

} // namespace

int Victory::countLevel8ForTeam(
    const std::vector<PlayerSnapshot> &players,
    std::string_view team) noexcept
{
    int count = 0;

    for (const PlayerSnapshot &player : players) {
        if (countsTowardVictory(player, team))
            ++count;
    }
    return count;
}

} // namespace Zappy::Server
