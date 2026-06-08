/*
** EPITECH PROJECT, 2026
** zappy_gotenku
** File description:
** Args.hpp (GUI)
*/

#ifndef ZAPPY_GUI_ARGS_HPP_
#define ZAPPY_GUI_ARGS_HPP_

#include <string_view>

namespace Zappy::Gui {

/**
 * @class Args
 * @brief Lit et verifie les args du client graphique
 */

class Args {
public:

    /**
     * @brief lit les args, throw un execption si token invalide
     * @param ac nb d'args
     * @param av array of args
     */
    explicit Args(int ac, char **av);

    [[nodiscard]] int getPort() const noexcept { return _port; }
    [[nodiscard]] std::string_view &getHost() const noexcept { return _host; }

    /**
     * @brief Mode d'emploie du client
     */
    static std::string_view getUsage() noexcept;

    /** 
     * @brief vbrai si (-help & --help) dans les args
    */
   static bool isHelp(int ac, char **av) noexcept;

private:
    void parseArgs(int ac, char **av);

    int _port = 0;
    std::string_view _host = "localhost";
};

} // namespace Zappy::Gui

#endif // ZAPPY_GUI_ARGS_HPP_