/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Client
*/

#ifndef ZAPPY_CLIENT_HPP_
#define ZAPPY_CLIENT_HPP_

#include "Command.hpp"
#include "Egg.hpp"
#include "Player.hpp"
#include "ReadBuffer.hpp"
#include "Socket.hpp"
#include <optional>
#include <queue>
#include <string>
#include <string_view>

namespace Zappy::Server {

/**
 * @class Client
 * @brief Un client connecte : socket, buffer, etat et file de cmds
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

    /**
     * @brief Fait apparaitre le joueur sur la carte (pos + dir aleatoires)
     */
    void spawnPlayer(int width, int height, int id);

    void spawnFromEgg(const Egg &egg, int id);

    /**
     * @brief Le joueur pilote par ce client, nullptr s'il n'existe pas
     */
    [[nodiscard]] Player *getPlayer() noexcept;
    [[nodiscard]] const Player *getPlayer() const noexcept;

    /**
     * @brief Met une cmd en file (max 10). False = file pleine
     *        La nouvelle cmd commence apres la derniere de la file.
     */
    bool enqueueCommand(
        std::string text, long nowMillis, long durationMillis);

    /**
     * @brief Vrai s'il y a au moins une cmd en file
     */
    [[nodiscard]] bool hasCommands() const noexcept
    {
        return !_commands.empty();
    }

    /**
     * @brief Heure prevue de la prochaine cmd (precondition : hasCommands)
     */
    [[nodiscard]] long getNextDeadlineMillis() const noexcept;

    /**
     * @brief Retire la premiere cmd de la file et la renvoie
     */
    [[nodiscard]] Command popCommand() noexcept;

    /**
     * @brief Consulte la premiere cmd sans la retirer 
     */
    [[nodiscard]] const Command &peekCommand() const noexcept;

    /**
     * @brief Vide la file de commandes en attente
     */
    void clearCommands() noexcept;

private:
    Socket _socket;
    ReadBuffer _readBuffer;
    State _state = State::AwaitingTeamName;
    std::string _teamName;
    std::queue<Command> _commands;
    std::optional<Player> _player;
};

} // namespace Zappy::Server

#endif
