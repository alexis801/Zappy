/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** CommandParser
*/

#include "CommandParser.hpp"
#include "Macros.hpp"

namespace Zappy::Server {

std::optional<int> CommandParser::getCommandCost(
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

} // namespace Zappy::Server
