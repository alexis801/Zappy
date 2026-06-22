/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Animation.cpp (GUI)
*/

#include "gui/Animation.hpp"

#include <iomanip>
#include <sstream>

namespace Zappy::Gui {

// prefix = assets/robot/idle, walk 00 each run -> idle, wlak +1 -> walk_01
bool Animation::load(const std::string &prefix, std::size_t count)
{
    _textures.clear();
    _textures.resize(count);
    for (std::size_t i = 0; i < count; ++i) {
        std::ostringstream path;
        path << prefix << std::setw(2) << std::setfill('0') << i << ".png";
        if (!_textures[i].loadFromFile(path.str()))
            return false;
        _textures[i].setSmooth(true);
    }
    return true;
}

} // namespace Zappy::Gui
