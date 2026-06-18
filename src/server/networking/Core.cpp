/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Core
*/

#include "CommandParser.hpp"
#include "Core.hpp"
#include "Macros.hpp"
#include "Tile.hpp"
#include <algorithm>
#include <cerrno>
#include <cstring>
#include <iostream>
#include <optional>
#include <string>
#include <utility>

namespace Zappy::Server {

namespace {

std::string buildHandshakeResponse(int slots, int width, int height)
{
    return std::to_string(slots) + "\n"
        + std::to_string(width) + " "
        + std::to_string(height) + "\n";
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

} // namespace

Core::Core(const Args &args)
    : _listener(Socket::listenOn(args.getPort())),
    _clock(args.getFrequency()),
    _spawner(42U),
    _map(args.getWidth(), args.getHeight()),
    _nextRespawnMillis(0),
    _lastLifeTickMillis(0),
    _nextPlayerId(0)
{
    for (const auto &name : args.getTeams())
        _teams.emplace_back(name, args.getClientsPerTeam());
    _spawner.initialSpawn(_map);
    _nextRespawnMillis = _clock.getDurationMillis(RESOURCE_SPAWN_INTERVAL);
    _lastLifeTickMillis = _clock.getElapsedMillis();
}

void Core::run()
{
    while (true) {
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
            std::cerr << "[gui " << client.getFd() << "] "
                      << *line << "\n";
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
            Command cmd = client.popCommand();

            executePlayerCommand(client, cmd);
        }
    }
}

void Core::executePlayerCommand(Client &client, const Command &cmd)
{
    Player *player = client.getPlayer();

    if (player == nullptr) {
        client.send("ko\n");
        return;
    }
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

        if (player != nullptr)
            broadcastToGui("pdi " + std::to_string(player->getId()) + "\n");
        Team *team = findTeam(client->getTeamName());

        if (team != nullptr)
            team->releaseSlot();
        client->send("dead\n");
    }
    removeClient(fd);
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
        client.send(buildHandshakeResponse(0, _map.getWidth(), _map.getHeight()));
        sendMapToGui(client);
        return true;
    }
    Team *team = findTeam(teamName);

    if (team == nullptr || !team->canAcceptClient())
        return false;
    team->claimSlot();
    client.setAsPlayer(teamName);
    client.spawnPlayer(_map.getWidth(), _map.getHeight(), _nextPlayerId++);
    client.send(buildHandshakeResponse(
        team->getRemainingSlots(), _map.getWidth(), _map.getHeight()));
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

} // namespace Zappy::Server
