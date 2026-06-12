/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** main.cpp
*/

#include "gui/Args.hpp"
#include "gui/Core.hpp"
#include "gui/Renderer.hpp"

#include <exception>
#include <iostream>

int main(int argc, char **argv)
{
    if (Zappy::Gui::Args::isHelp(argc, argv)) {
        std::cout << Zappy::Gui::Args::getUsage();
        return 0;
    }
    try {
        Zappy::Gui::Args args(argc, argv);
        Zappy::Gui::Core core(args);
        Zappy::Gui::Renderer renderer;

        while (renderer.isOpen()) {
            renderer.handleEvents();
            if (!core.update())
                break;
            renderer.draw(core.getWorld());
        }

    } catch (const std::exception &error) {
        std::cerr << error.what() << "\n";
        return 84;
    }
    return 0;
}