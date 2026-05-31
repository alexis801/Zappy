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
        Core core(args.getPort());

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
