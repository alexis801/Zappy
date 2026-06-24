/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Core
*/

#include "CommandParser.hpp"
#include "Core.hpp"
#include "Egg.hpp"
#include "GuiProtocol.hpp"
#include "Macros.hpp"
#include "Tile.hpp"
#include "Victory.hpp"
#include <algorithm>
#include <cerrno>
#include <cstring>
#include <iostream>
#include <optional>
#include <random>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <map>
#include <functional>

namespace Zappy::Server {

namespace {

constexpr unsigned DEFAULT_SPAWN_SEED = 42;

std::string buildHandshakeResponse(int slots, int width, int height)
{
    return std::to_string(slots) + "\n"
        + std::to_string(width) + " "
        + std::to_string(height) + "\n";
}

int signOf(int value) noexcept
{
    return (value > 0) - (value < 0);
}

int wrapDistance(int distance, int mapsize) noexcept
{
    distance %= mapsize;
    if (distance > mapsize / 2)
        distance -= mapsize;
    else if (distance < -mapsize / 2)
        distance += mapsize;
    return distance;
}

int soundDirection(const Player &source, const Player &receiver,
    int mapwidth, int mapheight)
{
    int globalDeltaX = wrapDistance(source.getX() - receiver.getX(), mapwidth);
    int globalDeltaY = wrapDistance(source.getY() - receiver.getY(), mapheight);
    int relativeForward = 0;
    int relativeRight = 0;
    static const int table[3][3] = {
        {4, 5, 6},
        {3, 0, 7},
        {2, 1, 8},
    };

    if (globalDeltaX == 0 && globalDeltaY == 0)
        return 0;
    switch (receiver.getDirection()) {
        case Direction::NORTH:
            relativeForward = -globalDeltaY;
            relativeRight = globalDeltaX;
            break;
        case Direction::EAST:
            relativeForward = globalDeltaX;
            relativeRight = globalDeltaY;
            break;
        case Direction::SOUTH:
            relativeForward = globalDeltaY;
            relativeRight = -globalDeltaX;
            break;
        case Direction::WEST:
            relativeForward = -globalDeltaX;
            relativeRight = -globalDeltaY;
            break;
    }
    return table[signOf(relativeForward) + 1][signOf(relativeRight) + 1];
}

std::string buildBctLine(int x, int y, const Tile &tile)
{
    std::string line = "bct " + std::to_string(x) + " "
        + std::to_string(y);

    for (std::size_t i = 0; i < RESOURCE_COUNT; ++i)
        line += " " + std::to_string(tile.get(static_cast<Resource>(i)));
    line += "\n";
    return line;
}

std::vector<PlayerSnapshot> gatherPlayerSnapshots(
    const std::vector<Client> &clients)
{
    std::vector<PlayerSnapshot> players;

    for (const Client &client : clients) {
        if (client.getState() != Client::State::Player)
            continue;
        const Player *player = client.getPlayer();

        if (player == nullptr)
            continue;
        players.push_back(PlayerSnapshot{
            client.getTeamName(), player->getLevel(), player->isAlive()});
    }
    return players;
}

} // namespace

Core::Core(const Args &args)
    : _listener(Socket::listenOn(args.getPort())),
    _clock(args.getFrequency()),
    _map(args.getWidth(), args.getHeight()),
    _spawner(DEFAULT_SPAWN_SEED),
    _nextRespawnMillis(0),
    _lastLifeTickMillis(0),
    _nextPlayerId(0),
    _nextEggId(0),
    _eggRng(DEFAULT_SPAWN_SEED)
{
    for (const auto &name : args.getTeams())
        _teams.emplace_back(name, args.getClientsPerTeam());
    _spawner.initialSpawn(_map);
    spawnInitialEggs(args.getClientsPerTeam());
    _nextRespawnMillis = _clock.getDurationMillis(RESOURCE_SPAWN_INTERVAL);
    _lastLifeTickMillis = _clock.getElapsedMillis();
}

void Core::spawnInitialEggs(int eggsPerTeam)
{
    std::uniform_int_distribution<int> distX(0, _map.getWidth() - 1);
    std::uniform_int_distribution<int> distY(0, _map.getHeight() - 1);

    for (const Team &team : _teams) {
        for (int i = 0; i < eggsPerTeam; ++i) {
            _eggs.emplace_back(_nextEggId++, team.getName(),
                distX(_eggRng), distY(_eggRng));
        }
    }
}

void Core::sendEggsToGui(Client &gui) const
{
    for (const Egg &egg : _eggs)
        gui.send(GuiProtocol::formatEnw(
            egg.getId(), 0, egg.getX(), egg.getY()));
}

void Core::broadcastEggsDestroyedOnTile(int x, int y)
{
    std::vector<int> destroyed = Egg::destroyOnTile(_eggs, x, y);

    for (int eggId : destroyed)
        broadcastToGui(GuiProtocol::formatEdi(eggId));
}

void Core::run()
{
    while (!_gameEnded) {
        std::vector<pollfd> fds = buildPollFds();

        if (::poll(fds.data(), fds.size(), getPollTimeoutMillis()) < 0) {
            std::cerr << "poll() failed: " << std::strerror(errno) << "\n";
            return;
        }
        handleEvents(fds);
        executeDueCommands();
        tickLife();
        tickResources();
    }
}

std::vector<pollfd> Core::buildPollFds() const
{
    std::vector<pollfd> fds;

    fds.push_back({_listener.getFd(), POLLIN, 0});
    for (const auto &client : _clients)
        fds.push_back({client.getFd(), POLLIN, 0});
    return fds;
}

int Core::getPollTimeoutMillis() const noexcept
{
    long soonest = _nextRespawnMillis;

    for (const auto &client : _clients) {
        if (!client.hasCommands())
            continue;
        long deadline = client.getNextDeadlineMillis();
        if (deadline < soonest)
            soonest = deadline;
    }
    return static_cast<int>(_clock.getRemainingMillis(soonest));
}

void Core::handleEvents(const std::vector<pollfd> &fds)
{
    if (fds[0].revents & POLLIN)
        acceptClients();
    for (std::size_t i = 1; i < fds.size(); ++i)
        handleClientEvent(fds[i]);
}

void Core::handleClientEvent(const pollfd &entry)
{
    Client *client = findClient(entry.fd);

    if (client == nullptr || entry.revents == 0)
        return;
    if (!client->receive())
        removeClient(entry.fd);
    else
        consumeLines(*client);
}

void Core::acceptClients()
{
    Socket client = _listener.acceptClient();

    while (client.isValid()) {
        std::cerr << "[serveur] client connecte (fd "
                  << client.getFd() << ")\n";
        _clients.emplace_back(std::move(client));
        _clients.back().send("WELCOME\n");
        client = _listener.acceptClient();
    }
}

void Core::consumeLines(Client &client)
{
    while (std::optional<std::string> line = client.popLine()) {
        if (client.getState() == Client::State::AwaitingTeamName) {
            if (!handleHandshake(client, *line)) {
                removeClient(client.getFd());
                return;
            }
        } else if (client.getState() == Client::State::Player) {
            handlePlayerLine(client, *line);
        } else {
            handleGuiLine(client, *line);
        }
    }
}

void Core::handlePlayerLine(Client &client, const std::string &line)
{
    std::optional<int> cost = CommandParser::getCommandCost(line);

    if (!cost.has_value()) {
        client.send("ko\n");
        return;
    }
    if (line == "Incantation" && !tryStartIncantation(client)) {
        client.send("ko\n");
        return;
    }
    long now = _clock.getElapsedMillis();
    long duration = _clock.getDurationMillis(*cost);

    if (!client.enqueueCommand(line, now, duration)) {
        std::cerr << "[client " << client.getFd()
                  << "] file pleine, commande ignoree\n";
        return;
    }
    std::cerr << "[client " << client.getFd() << "] " << line
              << " prevu a " << client.getNextDeadlineMillis() << "ms\n";
}

void Core::executeDueCommands()
{
    long now = _clock.getElapsedMillis();

    for (auto &client : _clients) {
        while (client.hasCommands()
            && client.getNextDeadlineMillis() <= now) {
            Player *player = client.getPlayer();
            if (player != nullptr && player->isFrozen()
                && client.peekCommand().text != "Incantation")
                break;
            Command cmd = client.popCommand();

            executePlayerCommand(client, cmd);
        }
    }
}

void Core::executeFork(Client &client, Player &player, std::string cmd)
{
    (void)cmd;
    Team *team = findTeam(client.getTeamName());

    if (team == nullptr) {
        client.send("ko\n");
        return;
    }
    if (!player.fork(*team, _eggs, _nextEggId)) {
        client.send("ko\n");
        return;
    }
    const Egg &egg = _eggs.back();

    broadcastToGui(GuiProtocol::formatPfk(player.getId()));
    broadcastToGui(GuiProtocol::formatEnw(
        egg.getId(), player.getId(), egg.getX(), egg.getY()));
    client.send("ok\n");
    return;
}

void Core::executeEject(Client &client, Player &player, std::string cmd)
{
    (void)client;
    (void)cmd;

    for (Client &cl : _clients) {
        Player *pl = cl.getPlayer();

        if (!pl || pl == &player)
            continue;
        if ((player.getX() != pl->getX()) || (player.getY() != pl->getY()))
            continue;
        pl->ejectFromTile(player.getDirection(), _map.getWidth(), _map.getHeight());
        cl.send("eject: " + player.directionToString() + "\n");
    }
}

void Core::executeTake(Client &client, Player &player, std::string cmd)
{
    (void)client;
    Tile tile = _map.at(player.getX(), player.getY());
    Resource res = searchResource(cmd);
    int nb = tile.get(res);
    if (nb > 0) {
        tile.remove(res, 1);
        player.addResource(res);
    }
}

void Core::executeSet(Client &client, Player &player, std::string cmd)
{
    (void)client;
    Tile tile = _map.at(player.getX(), player.getY());
    Resource res = searchResource(cmd);
    bool ret = player.removeResource(res);
    if (ret)
        tile.add(res, 1);
}

void Core::executeConnect_nbr(Client &client, Player &player, std::string cmd)
{
    (void)cmd;
    (void)player;
    Team *team = findTeam(client.getTeamName());
    client.send(std::to_string(
        team != nullptr ? team->getRemainingSlots() : 0) + "\n");
}

void Core::executePlayerCommand(Client &client, const Command &cmd)
{
    Player *player = client.getPlayer();
    using CommandHandler = void (Core::*)(Client &, Player &, std::string);

    std::map<std::string, CommandHandler> commands = {
        {"Fork", &Core::executeFork}, {"Incantation", &Core::completeIncantation},
        {"Eject", &Core::executeEject}, {"Take", &Core::executeTake},
        {"Set", &Core::executeSet}, {"Connect_nbr", &Core::executeConnect_nbr}
    };

    if (player == nullptr) {
        client.send("ko\n");
        return;
    }

    if (cmd.text.rfind("Broadcast", 0) == 0) {
        handleBroadcast(client, cmd.text);
        client.send("ok\n");
        return;
    }

    auto it = commands.find(getFirstCommand(cmd.text));
    if (it != commands.end())
        return (this->*(it->second))(client, *player, getRestCommand(cmd.text));

    CommandOutcome outcome = player->executeCommand(
        cmd.text, _map.getWidth(), _map.getHeight());

    if (outcome.kind == CommandKind::KO) {
        client.send("ko\n");
        return;
    }
    if (outcome.kind == CommandKind::INVENTORY) {
        client.send(outcome.text + "\n");
        return;
    }
    client.send("ok\n");
    broadcastToGui(GuiProtocol::formatPpo(player->getId(), *player));
}

void Core::handleBroadcast(Client &sender, const std::string &command)
{
    Player *src = sender.getPlayer();
    std::string::size_type pos = command.find(' ');
    std::string text;

    if (src == nullptr)
        return;
    if (pos != std::string::npos)
        text = command.substr(pos + 1);
    for (auto &client : _clients) {
        Player *receiver = client.getPlayer();

        if (client.getState() != Client::State::Player
            || client.getFd() == sender.getFd() || receiver == nullptr)
            continue;
        client.send("message " + std::to_string(soundDirection(
            *src, *receiver, _map.getWidth(), _map.getHeight()))
            + ", " + text + "\n");
    }
    broadcastToGuis(GuiProtocol::formatPbc(src->getId(), text));
}

std::vector<Player *> Core::findIncantationParticipants(
    int x, int y, int level)
{
    std::vector<Player *> result;

    for (auto &client : _clients) {
        if (client.getState() != Client::State::Player)
            continue;
        Player *player = client.getPlayer();
        if (player == nullptr)
            continue;
        if (player->getX() == x && player->getY() == y
            && player->getLevel() == level)
            result.push_back(player);
    }
    return result;
}

bool Core::meetsIncantationRequirements(
    const std::vector<Player *> &players, const Tile &tile,
    int level) noexcept
{
    if (level < 1 || level >= MAX_PLAYER_LEVEL)
        return false;
    const IncantationRequirement &req = INCANTATION_TABLE[level - 1];
    if (static_cast<int>(players.size()) < req.players)
        return false;
    for (std::size_t i = 0; i < req.stones.size(); ++i)
        if (tile.get(static_cast<Resource>(i + 1)) < req.stones[i])
            return false;
    return true;
}

bool Core::tryStartIncantation(Client &client)
{
    Player *starter = client.getPlayer();

    if (starter == nullptr)
        return false;
    int x = starter->getX();
    int y = starter->getY();
    int level = starter->getLevel();
    std::vector<Player *> participants = findIncantationParticipants(
        x, y, level);

    if (!meetsIncantationRequirements(participants, _map.at(x, y), level))
        return false;
    client.send("Elevation underway\n");
    client.clearCommands();
    std::vector<int> ids;
    for (Player *p : participants)
        ids.push_back(p->getId());
    broadcastToGui(GuiProtocol::formatPic(x, y, level, ids));
    for (Player *p : participants)
        p->setFrozen(true);
    return true;
}

std::vector<Player *> Core::gatherFrozenAt(int x, int y, int level)
{
    std::vector<Player *> result;

    for (auto &c : _clients) {
        if (c.getState() != Client::State::Player)
            continue;
        Player *p = c.getPlayer();
        if (p == nullptr || !p->isFrozen())
            continue;
        if (p->getX() == x && p->getY() == y && p->getLevel() == level)
            result.push_back(p);
    }
    return result;
}

void Core::completeIncantation(Client &client, Player &starter, std::string cmd)
{
    (void)cmd;
    (void)client;
    int x = starter.getX();
    int y = starter.getY();
    int level = starter.getLevel();
    std::vector<Player *> participants = gatherFrozenAt(x, y, level);
    bool success = meetsIncantationRequirements(
        participants, _map.at(x, y), level);

    for (Player *p : participants)
        p->setFrozen(false);
    if (success)
        finishIncantationSuccess(participants, x, y, level);
    else
        finishIncantationFailure(participants, x, y);
}

void Core::finishIncantationFailure(
    const std::vector<Player *> &players, int x, int y)
{
    for (Player *p : players) {
        Client *c = findPlayerById(p->getId());
        if (c != nullptr)
            c->send("ko\n");
    }
    broadcastToGui(GuiProtocol::formatPie(x, y, false));
}

void Core::finishIncantationSuccess(
    const std::vector<Player *> &players, int x, int y, int level)
{
    const IncantationRequirement &req = INCANTATION_TABLE[level - 1];
    int newLevel = level + 1;

    for (std::size_t i = 0; i < req.stones.size(); ++i)
        _map.at(x, y).remove(static_cast<Resource>(i + 1), req.stones[i]);
    broadcastTile(x, y);
    for (Player *p : players) {
        p->setLevel(newLevel);
        Client *c = findPlayerById(p->getId());
        if (c != nullptr)
            c->send("Current level: " + std::to_string(newLevel) + "\n");
        broadcastToGui(GuiProtocol::formatPlv(p->getId(), newLevel));
    }
    broadcastToGui(GuiProtocol::formatPie(x, y, true));
    checkVictory();
}

void Core::checkVictory()
{
    if (_gameEnded)
        return;
    std::optional<std::string> winner = Victory::detectWinner(
        gatherPlayerSnapshots(_clients), _teams);

    if (winner.has_value())
        endGame(*winner);
}

void Core::endGame(const std::string &winnerTeam)
{
    if (_gameEnded)
        return;
    _gameEnded = true;
    broadcastToGui(GuiProtocol::formatSeg(winnerTeam));
    std::cerr << "[serveur] victoire de l'equipe " << winnerTeam << "\n";
}

void Core::tickResources() noexcept
{
    long now = _clock.getElapsedMillis();

    if (now < _nextRespawnMillis)
        return;
    std::vector<std::pair<int, int>> changedTiles;

    _spawner.respawn(_map, &changedTiles);
    std::sort(changedTiles.begin(), changedTiles.end());
    changedTiles.erase(
        std::unique(changedTiles.begin(), changedTiles.end()),
        changedTiles.end());
    for (const auto &[x, y] : changedTiles)
        broadcastTile(x, y);
    while (_nextRespawnMillis <= now)
        _nextRespawnMillis += _clock.getDurationMillis(RESOURCE_SPAWN_INTERVAL);
}

void Core::tickLife() noexcept
{
    long now = _clock.getElapsedMillis();
    long elapsedMs = now - _lastLifeTickMillis;

    if (elapsedMs <= 0)
        return;
    _lastLifeTickMillis = now;
    int elapsedUnits = static_cast<int>(
        elapsedMs * _clock.getFrequency() / 1000);

    if (elapsedUnits <= 0)
        return;
    std::vector<int> deadClients;

    for (auto &client : _clients) {
        if (client.getState() != Client::State::Player)
            continue;
        Player *player = client.getPlayer();

        if (player == nullptr)
            continue;
        player->consumeFood(elapsedUnits);
        if (!player->isAlive())
            deadClients.push_back(client.getFd());
    }
    for (int fd : deadClients)
        killPlayer(fd);
}

void Core::killPlayer(int fd)
{
    Client *client = findClient(fd);

    if (client == nullptr)
        return;
    if (client->getState() == Client::State::Player) {
        Player *player = client->getPlayer();

        if (player != nullptr) {
            if (player->isFrozen())
                cancelIncantationAt(player->getX(), player->getY(),
                    player->getLevel());
            broadcastToGui(GuiProtocol::formatPdi(player->getId()));
        }
        Team *team = findTeam(client->getTeamName());

        if (team != nullptr)
            team->releaseSlot();
        client->send("dead\n");
    }
    removeClient(fd);
}

void Core::cancelIncantationAt(int x, int y, int level)
{
    std::vector<Player *> frozen = gatherFrozenAt(x, y, level);

    if (frozen.empty())
        return;
    for (Player *p : frozen) {
        p->setFrozen(false);
        Client *c = findPlayerById(p->getId());
        if (c != nullptr)
            c->send("ko\n");
    }
    broadcastToGui(GuiProtocol::formatPie(x, y, false));
}

void Core::sendMapToGui(Client &gui) const
{
    gui.send("msz " + std::to_string(_map.getWidth()) + " "
        + std::to_string(_map.getHeight()) + "\n");
    for (int y = 0; y < _map.getHeight(); ++y) {
        for (int x = 0; x < _map.getWidth(); ++x)
            gui.send(buildBctLine(x, y, _map.at(x, y)));
    }
}

void Core::broadcastToGui(const std::string &message)
{
    for (auto &client : _clients) {
        if (client.getState() == Client::State::Gui)
            client.send(message);
    }
}

void Core::broadcastTile(int x, int y)
{
    broadcastToGui(buildBctLine(x, y, _map.at(x, y)));
}

bool Core::handleHandshake(Client &client, const std::string &teamName)
{
    if (teamName == Zappy::GRAPHIC_TEAM_NAME) {
        client.setAsGui();
        sendGuiInitialState(client);
        sendEggsToGui(client);
        return true;
    }
    Team *team = findTeam(teamName);

    if (team == nullptr || !team->canAcceptClient())
        return false;
    team->claimSlot();
    int playerId = _nextPlayerId++;

    client.setAsPlayer(teamName);
    std::optional<Egg> hatched = Egg::pickRandomForTeam(
        _eggs, teamName, _eggRng);

    if (hatched.has_value()) {
        client.spawnFromEgg(*hatched, playerId);
        broadcastToGui(GuiProtocol::formatEbo(hatched->getId()));
    } else {
        client.spawnPlayer(_map.getWidth(), _map.getHeight(), playerId);
    }
    client.send(buildHandshakeResponse(
        team->getRemainingSlots(), _map.getWidth(), _map.getHeight()));
    if (client.getPlayer() != nullptr)
        broadcastToGuis(GuiProtocol::formatPnw(
            playerId, *client.getPlayer(), 1, teamName));
    return true;
}

Client *Core::findClient(int fd)
{
    for (auto &client : _clients)
        if (client.getFd() == fd)
            return &client;
    return nullptr;
}

Team *Core::findTeam(const std::string &name)
{
    for (auto &team : _teams)
        if (team.getName() == name)
            return &team;
    return nullptr;
}

void Core::removeClient(int fd)
{
    std::cerr << "[serveur] client deconnecte (fd " << fd << ")\n";
    std::erase_if(_clients,
        [fd](const Client &client) { return client.getFd() == fd; });
}

void Core::handleGuiLine(Client &client, const std::string &line)
{
    using GuiMember = void (Core::*)(Client &, std::istringstream &);
    static const std::unordered_map<std::string, GuiMember> handlers = {
        {"msz", &Core::respondMsz}, {"mct", &Core::respondMct},
        {"tna", &Core::respondTna}, {"sgt", &Core::respondSgt},
        {"sst", &Core::respondSst}, {"bct", &Core::respondBct},
        {"ppo", &Core::respondPpo}, {"plv", &Core::respondPlv},
        {"pin", &Core::respondPin},
    };
    std::istringstream iss(line);
    std::string command;

    iss >> command;
    auto handler = handlers.find(command);
    if (handler == handlers.end()) {
        client.send(GuiProtocol::formatSuc());
        return;
    }
    (this->*(handler->second))(client, iss);
}

void Core::respondPin(Client &client, std::istringstream &iss)
{
    char hash = 0;
    int playerId = 0;

    if (!(iss >> hash >> playerId) || hash != '#') {
        client.send(GuiProtocol::formatSbp());
        return;
    }
    Client *clientPlayer = findPlayerById(playerId);
    if (clientPlayer == nullptr || clientPlayer->getPlayer() == nullptr)
        client.send(GuiProtocol::formatSbp());
    else {
        const Player &player = *clientPlayer->getPlayer();
        client.send(GuiProtocol::formatPin(playerId, player, _map.at(player.getX(), player.getY())));
    }
}

void Core::respondMsz(Client &client, std::istringstream &)
{
    client.send(GuiProtocol::formatMsz(_map.getWidth(), _map.getHeight()));
}

void Core::respondMct(Client &client, std::istringstream &)
{
    for (int y = 0; y < _map.getHeight(); ++y)
        for (int x = 0; x < _map.getWidth(); ++x)
            client.send(GuiProtocol::formatBct(x, y, _map.at(x, y)));
}

void Core::respondTna(Client &client, std::istringstream &)
{
    for (const auto &team : _teams)
        client.send(GuiProtocol::formatTna(team.getName()));
}

void Core::respondSgt(Client &client, std::istringstream &)
{
    client.send(GuiProtocol::formatSgt(_clock.getFrequency()));
}

void Core::respondSst(Client &client, std::istringstream &iss)
{
    int frequency = 0;
    if (!(iss >> frequency) || frequency <= 0) {
        client.send(GuiProtocol::formatSbp());
        return;
    }
    client.send(GuiProtocol::formatSst(_clock, frequency));
}

void Core::respondBct(Client &client, std::istringstream &iss)
{
    int x = 0;
    int y = 0;

    if (!(iss >> x >> y)
        || x < 0 || y < 0 || x >= _map.getWidth() || y >= _map.getHeight()) {
        client.send(GuiProtocol::formatSbp());
        return;
    }
    client.send(GuiProtocol::formatBct(x, y, _map.at(x, y)));
}

void Core::respondPpo(Client &client, std::istringstream &iss)
{
    char hash = 0;
    int playerId = 0;

    if (!(iss >> hash >> playerId) || hash != '#') {
        client.send(GuiProtocol::formatSbp());
        return;
    }
    Client *player = findPlayerById(playerId);

    if (player == nullptr || player->getPlayer() == nullptr)
        client.send(GuiProtocol::formatSbp());
    else
        client.send(GuiProtocol::formatPpo(playerId, *player->getPlayer()));
}

void Core::respondPlv(Client &client, std::istringstream &iss)
{
    char hash = 0;
    int playerId = 0;

    if (!(iss >> hash >> playerId) || hash != '#') {
        client.send(GuiProtocol::formatSbp());
        return;
    }
    if (findPlayerById(playerId) != nullptr)
        client.send(GuiProtocol::formatPlv(playerId, 1));
    else
        client.send(GuiProtocol::formatSbp());
}

void Core::broadcastToGuis(std::string_view message)
{
    for (auto &client : _clients)
        if (client.getState() == Client::State::Gui)
            client.send(message);
}

Client *Core::findPlayerById(int playerId)
{
    for (auto &client : _clients)
        if (client.getState() == Client::State::Player
            && client.getPlayer() != nullptr
            && client.getPlayer()->getId() == playerId)
            return &client;
    return nullptr;
}

void Core::sendGuiInitialState(Client &client)
{
    std::istringstream empty;

    respondMsz(client, empty);
    respondSgt(client, empty);
    respondMct(client, empty);
    respondTna(client, empty);

    for (auto &other : _clients) {
        Player *player = other.getPlayer();
        if (other.getState() == Client::State::Player && player != nullptr)
            client.send(GuiProtocol::formatPnw(player->getId(), *player, player->getLevel(), other.getTeamName()));
    }
}

} // namespace Zappy::Server
