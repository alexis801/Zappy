/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Core
*/

#ifndef ZAPPY_CORE_HPP_
#define ZAPPY_CORE_HPP_

#include "Args.hpp"
#include "Client.hpp"
#include "Clock.hpp"
#include "Macros.hpp"
#include "Map.hpp"
#include "ResourceSpawner.hpp"
#include "Socket.hpp"
#include "Team.hpp"
#include <poll.h>
#include <string>
#include <vector>

namespace Zappy::Server {

/**
 * @class Core
 * @brief Ecoute les clients, fait tourner la loop poll et la poignee de main
 */
class Core {
public:
    explicit Core(const Args &args);

    void run();

private:
    [[nodiscard]] std::vector<pollfd> buildPollFds() const;
    [[nodiscard]] int getPollTimeoutMillis() const noexcept;
    void handleEvents(const std::vector<pollfd> &fds);
    void handleClientEvent(const pollfd &entry);
    void acceptClients();
    void consumeLines(Client &client);
    bool handleHandshake(Client &client, const std::string &teamName);
    void handlePlayerLine(Client &client, const std::string &line);
    void executeDueCommands();
    void executePlayerCommand(Client &client, const Command &cmd);
    void tickResources() noexcept;
    void tickLife() noexcept;
    void killPlayer(int fd);
    void sendMapToGui(Client &gui) const;
    void broadcastToGui(const std::string &message);
    void broadcastTile(int x, int y);
    void removeClient(int fd);

    [[nodiscard]] Client *findClient(int fd);
    [[nodiscard]] Team *findTeam(const std::string &name);

    Socket _listener;
    Clock _clock;
    ResourceSpawner _spawner;
    std::vector<Client> _clients;
    std::vector<Team> _teams;
    Map _map;
    long _nextRespawnMillis;
    long _lastLifeTickMillis;
    int _nextPlayerId;
};

} // namespace Zappy::Server

#endif
