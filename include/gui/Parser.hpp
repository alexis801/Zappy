/*
** EPITECH PROJECT, 2026
** zapp
** File description:
** parser.hpp (GUI)
*/

#ifndef ZAPPY_GUI_PARSER_HPP_
#define ZAPPY_GUI_PARSER_HPP_

#include "gui/World.hpp"

#include <string>

namespace Zappy::Gui {

/**
 * @class Parser
 * @brief Lit les lignes du protocol et met a jour le World
 */

class Parser {
public:
    void apply(const std::string &line, World &world) const;
};

} // namespace Zappy::Gui

#endif // ZAPPY_GUI_PARSER_HPP_