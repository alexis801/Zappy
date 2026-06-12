/*
** EPITECH PROJECT, 2026
** zapp
** File description:
** Core.hpp (GUI)
*/

#ifndef ZAPPY_GUI_CORE_HPP_
#define ZAPPY_GUI_CORE_HPP_

#include "gui/Args.hpp"
#include "gui/Connection.hpp"
#include "gui/Parser.hpp"
#include "gui/World.hpp"

#include <string>

namespace Zappy::Gui {

/**
 * @class Core
 * @brief Coeur du client graphique : conexion, handshake, poll loop
 */
class Core {
public:
    explicit Core(const Args &args);

    /**
     * @brief Boucle de receive data du serveur
     */
    void run();

private:
    bool update();
    void handleLine(const std::string &line);

    Connection _connection;
    bool _ready = false;
    World _world;
    Parser _parser;
};

} // namespace Zappy::Gui

#endif // ZAPPY_GUI_CORE_HPP_