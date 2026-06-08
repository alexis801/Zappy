/*
** EPITECH PROJECT, 2026
** zappy_gotenku
** File description:
** Args.cpp (GUI)
*/

#include "gui/Args.hpp"

#include <string_view>
#include <stdexcept>
#include <string>

namespace Zappy::Gui {

namespace {

// send value to string, raise if it's missing
std::string requireValue(std::string_view flag, const char *value)
{
    if (value == nullptr)
        throw std::invalid_argument(
            "Valeur manquante pour " + std::string(flag));
    return value;
}

// convert value to positive integer, raise if missing or wrong
int parsePos(std::string_view flag, const char *value)
{
    if (value == nullptr)
        throw std::invalid_argument("Missing value for" +std::string(flag));
    
    std::string text = value;
    std::size_t pos = 0;
    int result = 0;

    try {
        result = std::stoi(text, &pos);
    } catch (const std::exception &) {
        result = 0;
    }
    if (result <= 0 || pos != text.size())
        throw std::invalid_argument(std::string(flag) + " : positive integer expected");
    return result;
}

} // namespace

Args::Args(int ac, char **av)
{
    parseArgs(ac, av);
}

// parse args by pair
void Args::parseArgs(int ac, char **av)
{
    for (int i = 1; i < ac; i += 2)
        applyOption(av[i], av[i + 1]);
}

// apply the flag by value
void Args::applyOption(std::string_view flag, const char *value)
{
    if (flag == "-p")
        _port = parsePos(flag, value);
    else if (flag == "-h")
        _host = requireValue(flag, value);
    else
        throw std::invalid_argument("Unknown option :" + std::string(flag));
}

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