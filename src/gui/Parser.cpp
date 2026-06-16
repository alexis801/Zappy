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

bool readId(std::istringstream &stream, int &id)
{
    std::string token;

    if (!(stream >> token) || token.size() < 2 || token[0] != '#')
        return false;
    std::istringstream ids(token.substr(1));
    return static_cast<bool>(ids >> id);
}

void applyMsz(std::istringstream &stream, World &world)
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
void applyPnw(std::istringstream &stream, World &world)
{
    Player player;

    if (readId(stream, player.id) && (stream >> player.x >> player.y >> player.orientation >> player.level >> player.team))
        world.addPlayer(player);
}

// ppo #n x y o position / orientation
void applyPpo(std::istringstream &stream, World &world)
{
    int id = 0;
    int x = 0;
    int y = 0;
    int orientation = 0;

    if (readId(stream, id) && (stream >> x >> y >> orientation))
        world.movePlayer(id, x, y, orientation);
}

// plv #n l niveau du joueur
void applyPlv(std::istringstream &stream, World &world)
{
    int id = 0;
    int level = 0;

    if (readId(stream, id) && (stream >> level))
        world.setPlayerLevel(id, level);
}

// pdi #n mort du joueur
void applyPdi(std::istringstream &stream, World &world)
{
    int id = 0;

    if (readId(stream, id))
        world.removePlayer(id);
}

} // anonymous namespace


void Parser::apply(const std::string &line, World &world) const
{
    std::istringstream stream(line);
    std::string command;

    stream >> command;
    if (command == "msz")
        applyMsz(stream, world);
    else if (command == "bct")
        applyBct(stream, world);
    else if (command == "pnw")
        applyPnw(stream, world);
    else if (command == "ppo")
        applyPpo(stream, world);
    else if (command == "plv")
        applyPlv(stream, world);
    else if (command == "pdi")
        applyPdi(stream, world);
}

} // namespace Zappy::Gui