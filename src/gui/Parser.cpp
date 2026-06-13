/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Parser.cpp (GUI)
*/

#include "gui/Parser.hpp"

#include <sstream>
#include <string>

namespace Zappy::Gui {

void Parser::apply(const std::string &line, World &world) const
{
    std::istringstream stream(line);
    std::string command;

    stream >> command;
    if (command == "msz") {
        int width = 0;
        int height = 0;

        if (stream >> width >> height) 
            world.setSize(width, height);
    } else if (command == "bct") {
        int x = 0;
        int y = 0;
        world::Tile tile{};

        stream >> x >> y;
        for (int &quantity : tile)
            stream >> quantity;
        if (stream)
            world.setTile(x, y, tile);
    }
}

} // namespace Zappy::Gui