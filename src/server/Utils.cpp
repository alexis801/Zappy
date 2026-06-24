/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Utils
*/

#include "Macros.hpp"

#include <unordered_map>
#include <iostream>
#include <sstream>

namespace Zappy {

Resource searchResource(const std::string &res)
{
    static const std::unordered_map<std::string, Resource> resources = {
        {"Food", Resource::FOOD}, {"Linemate", Resource::LINEMATE},
        {"Deraumere", Resource::DERAUMERE}, {"Sibur", Resource::SIBUR},
        {"Mendiane", Resource::MENDIANE}, {"Phiras", Resource::PHIRAS},
        {"Thystame", Resource::THYSTAME}
    };

    auto it = resources.find(res);
    if (it == resources.end())
        throw std::invalid_argument("Unknown resource: " + res);
    return it->second;
}


std::string getRestCommand(const std::string &cmd)
{
    size_t pos = cmd.find(' ');
    if (pos != std::string::npos)
        return cmd.substr(pos + 1);
    return "";
}

std::string getFirstCommand(const std::string &cmd)
{
    std::string firstCmd;
    std::istringstream iss(cmd);
    iss >> firstCmd;
    return firstCmd;
}

} // namespace Zappy
