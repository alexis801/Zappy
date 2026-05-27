/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** main
*/

#include "Args.hpp"
#include <exception>
#include <iostream>

int main(int argc, char **argv)
{
    if (Zappy::Server::Args::isHelpRequested(argc, argv)) {
        std::cout << Zappy::Server::Args::getUsage();
        return 0;
    }
    try {
        const Zappy::Server::Args args(argc, argv);

        std::cout << "port=" << args.getPort()
                  << " carte=" << args.getWidth() << "x" << args.getHeight()
                  << " clients=" << args.getClientsPerTeam()
                  << " f=" << args.getFrequency()
                  << " equipes=" << args.getTeams().size() << "\n";
    } catch (const std::exception &e) {
        std::cerr << e.what() << "\n" << Zappy::Server::Args::getUsage();
        return 84;
    }
    return 0;
}
