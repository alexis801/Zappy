/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** CommandParser
*/

#include "CommandParser.hpp"
#include "Macros.hpp"

#include <sstream>
#include <string>
#include <map>
#include <functional>

namespace Zappy::Server {

std::optional<int> CommandParser::ForwardCost(std::string_view line) noexcept
{
    if (line == "Forward")
        return action_cost::FORWARD;
    return std::nullopt;
}

std::optional<int> CommandParser::LeftCost(std::string_view line) noexcept
{
    if (line == "Left")
        return action_cost::LEFT;
    return std::nullopt;
}

std::optional<int> CommandParser::RightCost(std::string_view line) noexcept
{
    if (line == "Right")
        return action_cost::RIGHT;
    return std::nullopt;
}

std::optional<int> CommandParser::InventoryCost(std::string_view line) noexcept
{
    if (line == "Inventory")
        return action_cost::INVENTORY;
    return std::nullopt;
}

std::optional<int> CommandParser::getCommandCost(
    std::string_view line) noexcept
{
    std::map<std::string_view, std::function<std::optional<int>(std::string_view line)>> commands = {
        {"Forward", ForwardCost}, {"Left", LeftCost}, {"Right", RightCost},
        {"Inventory", InventoryCost}
    };

    auto it = commands.find(line);
    if (it != commands.end())
        return it->second(line);
    return std::nullopt;
}

} // namespace Zappy::Server
