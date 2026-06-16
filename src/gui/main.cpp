/*
** EPITECH PROJECT, 2026
** zappy_gotenku
** File description:
** main (GUI)
*/

#include "gui/Args.hpp"
#include "gui/Core.hpp"
#include "gui/Menu.hpp"
#include "gui/Renderer.hpp"

#include <exception>
#include <iostream>
#include <optional>

namespace {

using Zappy::Gui::Args;
using Zappy::Gui::Core;
using Zappy::Gui::Menu;
using Zappy::Gui::Renderer;

sf::Vector2f at(const sf::Event::MouseMoveEvent &m)
{
    return {static_cast<float>(m.x), static_cast<float>(m.y)};
}

sf::Vector2f at(const sf::Event::MouseButtonEvent &m)
{
    return {static_cast<float>(m.x), static_cast<float>(m.y)};
}

void onMenuAction(Menu::ButtonAction action, const Args &args, std::optional<Core> &core, Renderer &renderer)
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
    // Menu::ButtonAction::Options a implementer
}

void handleEvents(Renderer &renderer, Menu &menu, const Args &args, std::optional<Core> &core)
{
    sf::Event event;

    while (renderer.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            renderer.close();
        else if (core)
            continue; // en jeu : pas d'interaction menu
        else if (event.type == sf::Event::MouseMoved)
            menu.updateHover(at(event.mouseMove));
        else if (event.type == sf::Event::MouseButtonPressed)
            menu.press(at(event.mouseButton));
        else if (event.type == sf::Event::MouseButtonReleased)
            onMenuAction(menu.release(at(event.mouseButton)), args, core, renderer);
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