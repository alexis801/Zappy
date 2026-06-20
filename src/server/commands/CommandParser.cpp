/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** CommandParser
*/

#include "CommandParser.hpp"
#include "Macros.hpp"

namespace Zappy::Server {

std::optional<int> CommandParser::getMovementCost(
    std::string_view line) noexcept
{
    if (line == "Forward")
        return action_cost::FORWARD;
    if (line == "Left")
        return action_cost::LEFT;
    if (line == "Right")
        return action_cost::RIGHT;
    return std::nullopt;
}

std::optional<int> CommandParser::getCommandCost(
    std::string_view line) noexcept
{
    std::optional<int> movement = getMovementCost(line);

    if (movement.has_value())
        return movement;
    if (line == "Inventory")
        return action_cost::INVENTORY;
    if (line == "Look")
        return action_cost::LOOK;
    return std::nullopt;
}

} // namespace Zappy::Server
