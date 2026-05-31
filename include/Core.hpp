/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Core
*/

#ifndef ZAPPY_CORE_HPP_
#define ZAPPY_CORE_HPP_

#include "Client.hpp"
#include "Socket.hpp"
#include <poll.h>
#include <vector>

namespace Zappy::Server {

/**
 * @class Core
 * @brief Ecoute les clients et fait tourner la loop poll du serveur
 */
class Core {
public:
    explicit Core(int port);

    void run();

private:
    [[nodiscard]] std::vector<pollfd> buildPollFds() const;
    void handleEvents(const std::vector<pollfd> &fds);
    void handleClientEvent(const pollfd &entry);
    void acceptClients();
    void consumeLines(Client &client);
    [[nodiscard]] Client *findClient(int fd);
    void removeClient(int fd);

    Socket _listener;
    std::vector<Client> _clients;
};

} // namespace Zappy::Server

#endif
