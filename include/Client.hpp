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
 * @brief Un client connecte : socket, buffer et etat
 */
class Client {
public:
    enum class State {
        AwaitingTeamName,
        Player,
        Gui,
    };

    explicit Client(Socket socket) noexcept;

    [[nodiscard]] int getFd() const noexcept { return _socket.getFd(); }
    [[nodiscard]] State getState() const noexcept { return _state; }
    [[nodiscard]] const std::string &getTeamName() const noexcept
    {
        return _teamName;
    }

    /**
     * @brief Lit les data dispo ; renvoie false si deconnecte
     */
    bool receive();

    /**
     * @brief Envoie des data au client ; renvoie false si erreur
     */
    bool send(std::string_view data);

    /**
     * @brief Ajoute des data au buffer
     */
    void feed(std::string_view data);

    /**
     * @brief Retire la prochaine ligne complete, si elle existe
     */
    [[nodiscard]] std::optional<std::string> popLine();

    /**
     * @brief Def le client comme joueur d'une equipe
     */
    void setAsPlayer(std::string teamName) noexcept;

    /**
     * @brief Def le client comme interface graphique
     */
    void setAsGui() noexcept;

private:
    Socket _socket;
    std::string _readBuffer;
    State _state = State::AwaitingTeamName;
    std::string _teamName;
};

} // namespace Zappy::Server

#endif
