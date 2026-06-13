/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** main
*/

#include "Args.hpp"
#include "Core.hpp"
#include <exception>
#include <iostream>
#include <stdexcept>

namespace Zappy::Server {

namespace {

void printConfig(const Args &args)
{
    std::cerr << "Server: port=" << args.getPort()
              << " map=" << args.getWidth() << "x" << args.getHeight()
              << " clients=" << args.getClientsPerTeam()
              << " freq=" << args.getFrequency() << "\n";
    std::cerr << "Teams:";
    for (const auto &team : args.getTeams())
        std::cerr << " " << team;
    std::cerr << "\n";
}

} // namespace

} // namespace Zappy::Server

int main(int argc, char **argv)
{
    using Zappy::Server::Args;
    using Zappy::Server::Core;

    if (Args::isHelpRequested(argc, argv)) {
        std::cout << Args::getUsage();
        return 0;
    }
    try {
        const Args args(argc, argv);
        Core core(args);

        Zappy::Server::printConfig(args);
        core.run();
    } catch (const std::invalid_argument &e) {
        std::cerr << e.what() << "\n" << Args::getUsage();
        return 84;
    } catch (const std::exception &e) {
        std::cerr << e.what() << "\n";
        return 84;
    }
    return 0;
}
