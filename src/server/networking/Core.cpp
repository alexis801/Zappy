/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Core
*/

#include "CommandParser.hpp"
#include "Core.hpp"
#include "GuiProtocol.hpp"
#include "Macros.hpp"
#include <cerrno>
#include <cstring>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>

namespace Zappy::Server {

namespace {

constexpr unsigned DEFAULT_SPAWN_SEED = 42;

std::string buildHandshakeResponse(int slots, int width, int height)
{
    return std::to_string(slots) + "\n"
        + std::to_string(width) + " "
        + std::to_string(height) + "\n";
}

} // namespace

Core::Core(const Args &args)
    : _listener(Socket::listenOn(args.getPort())),
    _map(args.getWidth(), args.getHeight()), _spawner(DEFAULT_SPAWN_SEED),
    _width(args.getWidth()), _height(args.getHeight())
{
    for (const auto &name : args.getTeams())
        _teams.emplace_back(name, args.getClientsPerTeam());
    _spawner.initialSpawn(_map);
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
    long soonest = -1;

    for (const auto &client : _clients) {
        if (!client.hasCommands())
            continue;
        long deadline = client.getNextDeadlineMillis();
        if (soonest < 0 || deadline < soonest)
            soonest = deadline;
    }
    if (soonest < 0)
        return -1;
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
        cmd.text, _width, _height);

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

bool Core::handleHandshake(Client &client, const std::string &teamName)
{
    if (teamName == Zappy::GRAPHIC_TEAM_NAME) {
        client.setAsGui();
        client.send(buildHandshakeResponse(0, _width, _height));
        sendGuiInitialState(client);
        return true;
    }
    Team *team = findTeam(teamName);

    if (team == nullptr || !team->canAcceptClient())
        return false;
    team->claimSlot();
    client.setAsPlayer(teamName);
    client.spawnPlayer(_width, _height);
    client.send(buildHandshakeResponse(
        team->getRemainingSlots(), _width, _height));
    if (client.getPlayer() != nullptr)
        broadcastToGuis(GuiProtocol::formatPnw(
            client.getFd(), *client.getPlayer(), 1, teamName));
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
        {"bct", &Core::respondBct}, {"ppo", &Core::respondPpo},
        {"plv", &Core::respondPlv}, {"plv", &Core::respondPin},
    };
    std::istringstream iss(line);
    std::string command;

    iss >> command;
    auto handler = handlers.find(command);
    if (handler == handlers.end()) {
        client.send("suc\n");
        return;
    }
    (this->*(handler->second))(client, iss);
}

void Core::respondPin(Client &client, std::istringstream &iss)
{
    char hash = 0;
    int playerId = 0;

    if (!(iss >> hash >> playerId) || hash != '#') {
        client.send("sbp\n");
        return;
    }
    Client *clientPlayer = findPlayerById(playerId);
    if (clientPlayer == nullptr || clientPlayer->getPlayer() == nullptr)
        client.send("pin\n");
    else {
        const Player &player = *clientPlayer->getPlayer();
        client.send(GuiProtocol::formatPin(playerId, player, _map.at(player.getX(), player.getY())));
    }
}

void Core::respondMsz(Client &client, std::istringstream &)
{
    client.send(GuiProtocol::formatMsz(_width, _height));
}

void Core::respondMct(Client &client, std::istringstream &)
{
    for (int y = 0; y < _height; ++y)
        for (int x = 0; x < _width; ++x)
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
    if (!iss >> frequency) {
        client.send("sst\n");
        return;
    }
    client.send(GuiProtocol::formatSst(_clock, frequency));
}

void Core::respondBct(Client &client, std::istringstream &iss)
{
    int x = 0;
    int y = 0;

    if (!(iss >> x >> y)
        || x < 0 || y < 0 || x >= _width || y >= _height) {
        client.send("sbp\n");
        return;
    }
    client.send(GuiProtocol::formatBct(x, y, _map.at(x, y)));
}

void Core::respondPpo(Client &client, std::istringstream &iss)
{
    char hash = 0;
    int playerId = 0;

    if (!(iss >> hash >> playerId) || hash != '#') {
        client.send("sbp\n");
        return;
    }
    Client *player = findPlayerById(playerId);

    if (player == nullptr || player->getPlayer() == nullptr)
        client.send("sbp\n");
    else
        client.send(GuiProtocol::formatPpo(playerId, *player->getPlayer()));
}

void Core::respondPlv(Client &client, std::istringstream &iss)
{
    char hash = 0;
    int playerId = 0;

    if (!(iss >> hash >> playerId) || hash != '#') {
        client.send("sbp\n");
        return;
    }
    if (findPlayerById(playerId) != nullptr)
        client.send(GuiProtocol::formatPlv(playerId, 1));
    else
        client.send("sbp\n");
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
        if (client.getFd() == playerId
            && client.getState() == Client::State::Player)
            return &client;
    return nullptr;
}

void Core::sendGuiInitialState(Client &client)
{
    std::istringstream empty;

    respondMsz(client, empty);
    respondTna(client, empty);
    respondMct(client, empty);
}

} // namespace Zappy::Server
