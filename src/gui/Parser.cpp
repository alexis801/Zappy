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

namespace {

void applyMsz(std::istringstream &stream, int &id)
{    
    int width = 0;
    int height = 0;

    if (stream >> width >> height)
        world.setSize(width, height);
}

void applyBct(std::istringstream &stream, World &world)
{
    int x = 0;
    int y = 0;
    World::Tile tile{};

    stream >> x >> y;
    for (int &quantity : tile)
        stream >> quantity;
    if (stream)
        world.setTile(x, y, tile);
}

} // anonymous namespace

void Parser::apply(const std::string &line, World &world) const
{
    std::istringstream stream(line);
    std::string command;

    stream >> command;
    if (command == "msz") {
        applyMsz(stream, world);
    else if (command == "bct")
        applyBct(stream, world);
    }
}

} // namespace Zappy::Gui