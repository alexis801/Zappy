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

std::optional<int> CommandParser::LookCost() noexcept
{
    return action_cost::LOOK;
}

std::optional<int> CommandParser::InventoryCost() noexcept
{
    return action_cost::INVENTORY;
}

std::optional<int> CommandParser::BroadcastCost() noexcept
{
    return action_cost::BROADCAST;
}

std::optional<int> CommandParser::ForkCost() noexcept
{
    return action_cost::FORK;
}

std::optional<int> CommandParser::EjectCost() noexcept
{
    return action_cost::EJECT;
}

std::optional<int> CommandParser::TakeCost() noexcept
{
    return action_cost::TAKE;
}

std::optional<int> CommandParser::SetCost() noexcept
{
    return action_cost::SET;
}

std::optional<int> CommandParser::IncantationCost() noexcept
{
    return action_cost::INCANTATION;
}

std::optional<int> CommandParser::ConnectNbrCost() noexcept
{
    return action_cost::CONNECT_NBR;
}

std::optional<int> CommandParser::getCommandCost(std::string_view line) noexcept
{
    std::map<std::string_view, std::function<std::optional<int>()>> commands = {
        {"Forward", ForwardCost}, {"Left", LeftCost}, {"Right", RightCost},
        {"Inventory", InventoryCost}, {"Look", LookCost}, {"Broadcast", BroadcastCost},
        {"Fork", ForkCost}, {"Eject", EjectCost}, {"Take", TakeCost},
        {"Set", SetCost}, {"Incantation", IncantationCost},
        {"Connect_nbr", ConnectNbrCost}
    };

    std::string_view commandName = line.substr(0, line.find(' '));
    auto matchedCommand = commands.find(commandName);
    if (matchedCommand != commands.end())
        return matchedCommand->second();
    return std::nullopt;
}

} // namespace Zappy::Server
