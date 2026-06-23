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
    [[nodiscard]] static std::optional<int> ForwardCost() noexcept;
    [[nodiscard]] static std::optional<int> LeftCost() noexcept;
    [[nodiscard]] static std::optional<int> RightCost() noexcept;
    [[nodiscard]] static std::optional<int> LookCost() noexcept;
    [[nodiscard]] static std::optional<int> InventoryCost() noexcept;
    [[nodiscard]] static std::optional<int> BroadcastCost() noexcept;
    [[nodiscard]] static std::optional<int> ForkCost() noexcept;
    [[nodiscard]] static std::optional<int> EjectCost() noexcept;
    [[nodiscard]] static std::optional<int> TakeCost() noexcept;
    [[nodiscard]] static std::optional<int> SetCost() noexcept;
    [[nodiscard]] static std::optional<int> IncantationCost() noexcept;
    [[nodiscard]] static std::optional<int> ConnectNbrCost() noexcept;

};

} // namespace Zappy::Server

#endif
