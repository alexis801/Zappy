/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Client
*/

#ifndef ZAPPY_CLIENT_HPP_
#define ZAPPY_CLIENT_HPP_

#include "Socket.hpp"
#include <optional>
#include <string>
#include <string_view>

namespace Zappy::Server {

/**
 * @class Client
 * @brief Un client connecte : sa socket et son buffer
 */
class Client {
public:
    explicit Client(Socket socket) noexcept;

    [[nodiscard]] int getFd() const noexcept { return _socket.getFd(); }

    /**
     * @brief Lit les data dispo ; renvoie false si deconnecte
     */
    bool receive();

    /**
     * @brief Ajoute des data au buffer
     */
    void feed(std::string_view data);

    /**
     * @brief Retire la prochaine ligne complete, si elle existe
     */
    [[nodiscard]] std::optional<std::string> popLine();

private:
    Socket _socket;
    std::string _readBuffer;
};

} // namespace Zappy::Server

#endif
