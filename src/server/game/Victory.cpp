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

std::optional<std::string> Victory::detectWinner(
    const std::vector<PlayerSnapshot> &players,
    const std::vector<Team> &teams) noexcept
{
    for (const Team &team : teams) {
        if (countLevel8ForTeam(players, team.getName())
            >= Zappy::VICTORY_REQUIRED_PLAYERS)
            return team.getName();
    }
    return std::nullopt;
}

} // namespace Zappy::Server
