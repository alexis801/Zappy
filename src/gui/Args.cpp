/*
** EPITECH PROJECT, 2026
** zappy_gotenku
** File description:
** Args.cpp (GUI)
*/

#include "gui/Args.hpp"

#include <string_view>

namespace Zappy::Gui {

std::string_view Args::getUsage() noexcept
{
    return "USAGE: ./zappy_gui -p port -h machine\n";
}

bool Args::isHelp(int ac, char **av) noexcept
{
    for (int i = 1; i < ac; ++i) {
        std::string_view arg = av[i];
        if (arg == "-help" || arg == "--help")
            return true;
    }
    return false;
}

} // namespace Zappy::Gui