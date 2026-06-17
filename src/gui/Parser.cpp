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

// tna N nom d'equipe
void applyTna(std::istringstream &stream, World &world)
{
    std::string name;

    if (stream >> name)
        world.addTeam(name);
}

// enw #e #n x y -> oeuf pondu par le joueur n
void applyEnw(std::istringstream &stream, World &world)
{
    Egg egg;

    if (readId(stream, egg.id) && readId(stream, egg.PLayerId) && (stream >> egg.x >> egg.y))
        world.addEgg(egg);
}

// ebo #e player eclot l'oeuf -> oeuf disparait
void applyEbo(std::istringstream &stream, World &world)
{
    int id = 0;

    if (readId(stream, id))
        world.removeEgg(id);
}

// edi #e mort d'un oeuf
void applyEdi(std::istringstream &stream, World &world)
{
    int id = 0;

    if (readId(stream, id))
        world.removeEgg(id);
}

//pin #n x y q0 q6 inventaire du joeur
void applyPin(std::istringstream &stream, World &world)
{
    int id = 0;
    int x = 0;
    int y = 0;
    std::array<int, Zappy::RESOURCE_COUNT> inventory{};

    if (!readId(stream, id) || !(stream >> x >> y))
        return;
    for (int &quantity : inventory)
        if (!(stream >> quantity))
            return;
    world.setPLayerInventory(id, x, y, inventory);
}

// void applyPic(std::istringstream &stream, World &world)

// void applyPie(std::istringstream &stream, World &world)

// void applySgt(std::istringstream &stream, World &world)

// void applySst(std::istringstream &stream, World &world)

// void applySeg(std::istringstream &stream, World &world)

// void applySmg(std::istringstream &stream, World &world)

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
    else if (command == "tna")
        applyTna(stream, world);
    else if (command == "enw")
        applyEnw(stream, world);
    else if (command == "ebo")
        applyEbo(stream, world);
    else if (command == "edi")
        applyEdi(stream, world);
    else if (command == "pin")
        applyPin(stream, world);
}

} // namespace Zappy::Gui