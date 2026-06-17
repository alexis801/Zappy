/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** PlayerVisual.hpp (GUI)
*/

#ifndef ZAPPY_GUI_PLAYERVISUAL_HPP_
#define ZAPPY_GUI_PLAYERVISUAL_HPP_

#include "gui/Animation.hpp"

#include <SFML/Graphics.hpp>

namespace Zappy::Gui {

enum class AnimState { Idle, Walk };

struct RobotAssets {

    Animation idle;
    Animation walk;

    bool load()
    {
        return idle.load("assets/robot/idle_", 3) && walk.load("assets/robot/walk_", 20);
    }

};

struct PlayerVisual {

    sf::Vector2f pos; // position pixel actuelle
    sf::Vector2f target; // position pixel voulu
    int orientation = 1;
    AnimState state = AnimState::Idle;
    std::size_t frame = 0;
    float timer = 0.f;
    bool initialized = false;
};

} // namespace Zappy::Gui
#endif