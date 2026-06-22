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
#include "Egg.hpp"
#include "Macros.hpp"
#include "Map.hpp"
#include "Player.hpp"
#include "ResourceSpawner.hpp"
#include "Socket.hpp"
#include "Team.hpp"
#include "Tile.hpp"
#include <poll.h>
#include <random>
#include <sstream>
#include <string>
#include <string_view>
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
    void handleGuiLine(Client &client, const std::string &line);
    void sendGuiInitialState(Client &client);
    void respondMsz(Client &client, std::istringstream &iss);
    void respondMct(Client &client, std::istringstream &iss);
    void respondTna(Client &client, std::istringstream &iss);
    void respondSgt(Client &client, std::istringstream &iss);
    void respondBct(Client &client, std::istringstream &iss);
    void respondPpo(Client &client, std::istringstream &iss);
    void respondPlv(Client &client, std::istringstream &iss);
    void respondPin(Client &client, std::istringstream &);
    void respondSst(Client &client, std::istringstream &);
    void broadcastToGuis(std::string_view message);
    [[nodiscard]] Client *findPlayerById(int playerId);
    void executeDueCommands();
    void executePlayerCommand(Client &client, const Command &cmd);
    [[nodiscard]] bool tryStartIncantation(Client &client);
    [[nodiscard]] std::vector<Player *> findIncantationParticipants(
        int x, int y, int level);
    [[nodiscard]] bool meetsIncantationRequirements(
        const std::vector<Player *> &players, const Tile &tile,
        int level) noexcept;
    void tickResources() noexcept;
    void tickLife() noexcept;
    void killPlayer(int fd);
    void sendMapToGui(Client &gui) const;
    void broadcastToGui(const std::string &message);
    void broadcastTile(int x, int y);
    void removeClient(int fd);
    void spawnInitialEggs(int eggsPerTeam);
    void sendEggsToGui(Client &gui) const;
    void broadcastEggsDestroyedOnTile(int x, int y);

    [[nodiscard]] Client *findClient(int fd);
    [[nodiscard]] Team *findTeam(const std::string &name);

    Socket _listener;
    Clock _clock{DEFAULT_FREQUENCY};
    Map _map;
    ResourceSpawner _spawner;
    std::vector<Client> _clients;
    std::vector<Team> _teams;
    long _nextRespawnMillis;
    long _lastLifeTickMillis;
    int _nextPlayerId;
    std::vector<Egg> _eggs;
    int _nextEggId;
    std::mt19937 _eggRng;
};

} // namespace Zappy::Server

#endif
