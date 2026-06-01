/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Socket
*/

#ifndef ZAPPY_SOCKET_HPP_
#define ZAPPY_SOCKET_HPP_

#include <string_view>

namespace Zappy::Server {

/**
 * @class Socket
 * @brief Possede un descripteur de socket et le ferme tout seul
 */
class Socket {
public:
    Socket() noexcept = default;
    explicit Socket(int fd) noexcept;
    ~Socket();

    Socket(const Socket &) = delete;
    Socket &operator=(const Socket &) = delete;
    Socket(Socket &&other) noexcept;
    Socket &operator=(Socket &&other) noexcept;

    [[nodiscard]] int getFd() const noexcept { return _fd; }
    [[nodiscard]] bool isValid() const noexcept { return _fd >= 0; }

    /**
     * @brief Cree une socket TCP qui ecoute sur le port (leve si echec)
     */
    static Socket listenOn(int port);

    /**
     * @brief Accepte un client en attente (Socket invalide si aucun)
     */
    [[nodiscard]] Socket acceptClient() const;

    /**
     * @brief Envoie des datas ; renvoie false si erreur ou deconnecte
     */
    bool write(std::string_view data);

private:
    int _fd = -1;
};

} // namespace Zappy::Server

#endif
