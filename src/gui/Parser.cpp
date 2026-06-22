/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Parser.cpp (GUI)
*/

#include "gui/Parser.hpp"

#include <sstream>
#include <string>
#include <map>
#include <functional>

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

    if (readId(stream, egg.id) && readId(stream, egg.playerId) && (stream >> egg.x >> egg.y))
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
    world.setPlayerInventory(id, x, y, inventory);
}

// pic x y l #n debut incantation
void applyPic(std::istringstream &stream, World &world)
{
    int x = 0;
    int y = 0;

    if (stream >> x >> y)
        world.startIncantation(x, y);
}

// pie x y r fin incantation
void applyPie(std::istringstream &stream, World &world)
{
    int x = 0;
    int y = 0;

    if (stream >> x >> y)
        world.endIncantation(x, y);
}

// sgt T time unit actuel
void applySgt(std::istringstream &stream, World &world)
{
    int unit = 0;

    if (stream >> unit)
        world.setTimeUnit(unit);
}

// sst T nouveau time unit confirm par server
void applySst(std::istringstream &stream, World &world)
{
    int unit = 0;

    if (stream >> unit)
        world.setTimeUnit(unit);
}

// seg N fin de partie equipe gagnante N
void applySeg(std::istringstream &stream, World &world)
{
    std::string winner;

    if (stream >> winner)
        world.endGame(winner);
}

// smg M message du serveur
void applySmg(std::istringstream &stream, World &world)
{
    std::string message;

    std::getline(stream, message);
    if (!message.empty() && message.front() == ' ')
        message.erase(message.begin());
    world.setServerMessage(message);
}

} // anonymous namespace

void Parser::apply(const std::string &line, World &world) const
{
    std::map<std::string, std::function<void(std::istringstream &, World &)>> commands = {
        {"msz", applyMsz}, {"bct", applyBct}, {"pnw", applyPnw},
        {"ppo", applyPpo}, {"plv", applyPlv}, {"pdi", applyPdi},
        {"tna", applyTna}, {"enw", applyEnw}, {"ebo", applyEbo},
        {"edi", applyEdi}, {"pin", applyPin}, {"pic", applyPic},
        {"pie", applyPie}, {"sgt", applySgt}, {"sst", applySst},
        {"seg", applySeg}, {"smg", applySmg}
    };
    std::istringstream stream(line);
    std::string command;

    stream >> command;
    auto it = commands.find(command);
    if (it != commands.end())
        it->second(stream, world);
}

} // namespace Zappy::Gui