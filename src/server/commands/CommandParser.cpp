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

std::optional<int> CommandParser::ForwardCost() noexcept
{
    return action_cost::FORWARD;
}

std::optional<int> CommandParser::LeftCost() noexcept
{
    return action_cost::LEFT;
}

std::optional<int> CommandParser::RightCost() noexcept
{
    return action_cost::RIGHT;
}

std::optional<int> CommandParser::InventoryCost() noexcept
{
    return action_cost::INVENTORY;
}

std::optional<int> CommandParser::getCommandCost(std::string_view line) noexcept
{
    std::map<std::string_view, std::function<std::optional<int>()>> commands = {
        {"Forward", ForwardCost}, {"Left", LeftCost}, {"Right", RightCost},
        {"Inventory", InventoryCost}
    };

    auto it = commands.find(line);
    if (it != commands.end())
        return it->second();
    return std::nullopt;
}

} // namespace Zappy::Server
