/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Args
*/

#include "Args.hpp"
#include <set>
#include <stdexcept>
#include <string>
#include <unordered_map>

namespace Zappy::Server {

namespace {

int parsePositive(std::string_view flag, const char *value)
{
    if (value == nullptr)
        throw std::invalid_argument(
            "Missing value for " + std::string(flag));
    std::string text = value;
    std::size_t pos = 0;
    int result = 0;

    try {
        result = std::stoi(text, &pos);
    } catch (const std::exception &) {
        result = 0;
    }
    if (result <= 0 || pos != text.size())
        throw std::invalid_argument(
            std::string(flag) + ": positive integer expected");
    return result;
}

} // namespace

Args::Args(int argc, char **argv)
{
    parse(argc, argv);
}

void Args::parse(int argc, char **argv)
{
    static const std::unordered_map<std::string_view, int Args::*> options = {
        {"-p", &Args::_port}, {"-x", &Args::_width}, {"-y", &Args::_height},
        {"-c", &Args::_clientsPerTeam}, {"-f", &Args::_frequency},
    };

    for (int i = 1; i < argc; ++i) {
        std::string_view arg = argv[i];
        auto option = options.find(arg);
        if (option != options.end())
            this->*(option->second) = parsePositive(arg, argv[++i]);
        else if (arg == "-n")
            i = parseTeams(i + 1, argc, argv);
        else
            throw std::invalid_argument(
                "Unknown option: " + std::string(arg));
    }
    validate();
}

int Args::parseTeams(int start, int argc, char **argv)
{
    int i = start;

    while (i < argc && argv[i][0] != '-') {
        _teams.emplace_back(argv[i]);
        ++i;
    }
    return i - 1;
}

void Args::validate() const
{
    std::set<std::string> seen;

    if (_port == 0 || _width == 0 || _height == 0
        || _clientsPerTeam == 0 || _teams.empty())
        throw std::invalid_argument(
            "Required arguments missing (-p -x -y -n -c)");
    for (const auto &name : _teams)
        if (!seen.insert(name).second)
            throw std::invalid_argument(
                "Duplicate team name: " + name);
}

std::string_view Args::getUsage() noexcept
{
    return "USAGE: ./zappy_server -p port -x width -y height "
           "-n name1 name2 ... -c clientsNb -f freq\n";
}

bool Args::isHelpRequested(int argc, char **argv) noexcept
{
    for (int i = 1; i < argc; ++i) {
        std::string_view arg = argv[i];
        if (arg == "-help" || arg == "--help")
            return true;
    }
    return false;
}

} // namespace Zappy::Server
