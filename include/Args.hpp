/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Args
*/

#ifndef ZAPPY_SERVER_ARGS_HPP_
#define ZAPPY_SERVER_ARGS_HPP_

#include "Macros.hpp"
#include <string>
#include <string_view>
#include <vector>

namespace Zappy::Server {

/**
 * @class Args
 * @brief Lit et verifie les args du serveur
 */
class Args {
public:
    /**
     * @brief Lit et verifie les args ; leve une exception si pas valides
     * @param argc Nb d'args
     * @param argv Tableau des args
     */
    explicit Args(int argc, char **argv);

    [[nodiscard]] int getPort() const noexcept { return _port; }
    [[nodiscard]] int getWidth() const noexcept { return _width; }
    [[nodiscard]] int getHeight() const noexcept { return _height; }
    [[nodiscard]] int getFrequency() const noexcept { return _frequency; }

    [[nodiscard]] int getClientsPerTeam() const noexcept
    {
        return _clientsPerTeam;
    }

    [[nodiscard]] const std::vector<std::string> &getTeams() const noexcept
    {
        return _teams;
    }

    /**
     * @brief Le mode d'emploi du serveur
     */
    static std::string_view getUsage() noexcept;

    /**
     * @brief Vrai si (-help / --help) est dans les args
     */
    static bool isHelpRequested(int argc, char **argv) noexcept;

private:
    void parse(int argc, char **argv);
    int parseTeams(int start, int argc, char **argv);
    void validate() const;

    int _port = DEFAULT_PORT;
    int _width = DEFAULT_WIDTH;
    int _height = DEFAULT_HEIGHT;
    int _clientsPerTeam = DEFAULT_CLIENTS_PER_TEAM;
    int _frequency = DEFAULT_FREQUENCY;
    std::vector<std::string> _teams{std::string(DEFAULT_TEAM_NAME)};
    bool _teamsProvided = false;
};

} // namespace Zappy::Server

#endif
