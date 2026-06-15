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
#include <optional>
#include <iostream>

namespace {

    using Zappy::Gui::Args;
    using Zappy::Gui::Core;
    using Zappy::Gui::Menu;
    using Zappy::Gui::Renderer;

void onMenuClick(Menu::ButtonAction action, const Args &args, std::optional<Core> &core, Renderer &renderer)
{
    if (action == Menu::ButtonAction::Quit) {
        renderer.close();
        return;
    }
    if (action == Menu::ButtonAction::Play) {
        try {
            core.emplace(args);
        } catch (const std::exception &error) {
            std::cerr << "Connexion impossible: " << error.what() << "\n";
        }
    }
}

void handleEvents(Renderer &renderer, Menu &menu, const Args &args, std::optional<Core> &core)
{
    sf::Event event;

    while (renderer.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            renderer.close();
        else if (!core && event.type == sf::Event::MouseButtonPressed)
            onMenuClick(menu.handleClick({static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y)}), args, core, renderer);
    }
}

bool updateAndRender(Renderer &renderer, Menu &menu, std::optional<Core> &core)
{
    if (!core) {
        renderer.drawMenu(menu);
        return true;
    }
    if (!core->update())
        return false;
    renderer.draw(core->getWorld());
    return true;
}

void runApp(const Args &args)
{
    Renderer renderer;
    Menu menu;
    std::optional<Core> core;

    while (renderer.isOpen()) {
        handleEvents(renderer, menu, args, core);
        if (!renderer.isOpen())
            break;
        if (!updateAndRender(renderer, menu, core))
            break;
    }
}

} // anonymous namespace

int main(int argc, char **argv)
{
    if (Args::isHelp(argc, argv)) {
        std::cout << Args::getUsage();
        return 0;
    }
    try {
        runApp(Args(argc, argv));
    } catch (const std::exception &error) {
        std::cerr << error.what() << "\n";
        return 84;
    }
    return 0;
}