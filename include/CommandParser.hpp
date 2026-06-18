/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** CommandParser
*/

#ifndef ZAPPY_COMMAND_PARSER_HPP_
#define ZAPPY_COMMAND_PARSER_HPP_

#include <optional>
#include <string_view>

namespace Zappy::Server {

/**
 * @class CommandParser
 * @brief Reconnait les commandes joueur et leur cout en unites de temps
 */
class CommandParser {
public:
    /**
     * @brief Cout d'une commande, nullopt si inconnue
     */
    [[nodiscard]] static std::optional<int> getCommandCost(
        std::string_view line) noexcept;

private:
    [[nodiscard]] static std::optional<int> getMovementCost(
        std::string_view line) noexcept;
};

} // namespace Zappy::Server

#endif
