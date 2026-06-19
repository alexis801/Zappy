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
    [[nodiscard]] Client *findClient(int fd);
    [[nodiscard]] Team *findTeam(const std::string &name);
    void removeClient(int fd);

    Socket _listener;
    Clock _clock{DEFAULT_FREQUENCY};
    Map _map;
    ResourceSpawner _spawner;
    std::vector<Client> _clients;
    std::vector<Team> _teams;
    int _width;
    int _height;
};

} // namespace Zappy::Server

#endif
