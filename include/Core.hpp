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
    void handleEvents(const std::vector<pollfd> &fds);
    void handleClientEvent(const pollfd &entry);
    void acceptClients();
    void consumeLines(Client &client);
    bool handleHandshake(Client &client, const std::string &teamName);
    [[nodiscard]] Client *findClient(int fd);
    [[nodiscard]] Team *findTeam(const std::string &name);
    void removeClient(int fd);

    Socket _listener;
    std::vector<Client> _clients;
    std::vector<Team> _teams;
    int _width;
    int _height;
};

} // namespace Zappy::Server

#endif
