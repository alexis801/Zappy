/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** GuiProtocol
*/

#include "GuiProtocol.hpp"
#include "Macros.hpp"
#include <cstddef>
#include <string>

namespace Zappy::Server {

int GuiProtocol::getDirectionCode(Direction direction) noexcept
{
    switch (direction) {
        case Direction::NORTH: return 1;
        case Direction::EAST:  return 2;
        case Direction::SOUTH: return 3;
        case Direction::WEST:  return 4;
    }
    return 1;
}

std::string GuiProtocol::formatMsz(int width, int height)
{
    return "msz " + std::to_string(width) + " "
        + std::to_string(height) + "\n";
}

std::string GuiProtocol::formatBct(int x, int y, const Tile &tile)
{
    std::string out = "bct " + std::to_string(x) + " " + std::to_string(y);

    for (std::size_t i = 0; i < RESOURCE_COUNT; ++i)
        out += " " + std::to_string(
            tile.get(static_cast<Resource>(i)));
    out += "\n";
    return out;
}

std::string GuiProtocol::formatTna(std::string_view team)
{
    return "tna " + std::string(team) + "\n";
}

std::string GuiProtocol::formatSgt(int frequency)
{
    return "sgt " + std::to_string(frequency) + "\n";
}

std::string GuiProtocol::formatPnw(int playerId, const Player &player,
    int level, std::string_view team)
{
    return "pnw #" + std::to_string(playerId) + " "
        + std::to_string(player.getX()) + " "
        + std::to_string(player.getY()) + " "
        + std::to_string(getDirectionCode(player.getDirection())) + " "
        + std::to_string(level) + " "
        + std::string(team) + "\n";
}

std::string GuiProtocol::formatPpo(int playerId, const Player &player)
{
    return "ppo #" + std::to_string(playerId) + " "
        + std::to_string(player.getX()) + " "
        + std::to_string(player.getY()) + " "
        + std::to_string(getDirectionCode(player.getDirection())) + "\n";
}

std::string GuiProtocol::formatPlv(int playerId, int level)
{
    return "plv #" + std::to_string(playerId) + " "
        + std::to_string(level) + "\n";
}

std::string GuiProtocol::formatPin(int playerId, const Player &player, const Tile &tile)
{
    std::string out = "pin " + std::to_string(player.getX()) + " " + std::to_string(player.getY());

    for (std::size_t i = 0; i < RESOURCE_COUNT; ++i)
        out += " " + std::to_string(tile.get(static_cast<Resource>(i)));
    out += "\n";
    return out;
}

} // namespace Zappy::Server
