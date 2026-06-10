/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Command
*/

#ifndef ZAPPY_COMMAND_HPP_
#define ZAPPY_COMMAND_HPP_

#include <string>

namespace Zappy::Server {

/**
 * @struct Command
 * @brief Une commande en attente : son texte et l'heure prevue d'execution
 */
struct Command {
    std::string text;
    long deadlineMillis;
};

} // namespace Zappy::Server

#endif
