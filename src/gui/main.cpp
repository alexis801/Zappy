/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** main.cpp
*/

#include "gui/Args.hpp"
#include "gui/Core.hpp"
#include "gui/Renderer.hpp"
#include "gui/Menu.hpp"

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
        Zappy::Gui::Renderer renderer;
        Zappy::Gui::Menu menu;

        while (renderer.isOpen()) {
            renderer.handleEvents();
            renderer.drawMenu(menu);
        }

    } catch (const std::exception &error) {
        std::cerr << error.what() << "\n";
        return 84;
    }
    return 0;
}