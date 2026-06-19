/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** GuiProtocol
*/

#ifndef ZAPPY_GUI_PROTOCOL_HPP_
#define ZAPPY_GUI_PROTOCOL_HPP_

#include "Player.hpp"
#include "Tile.hpp"
#include <string>
#include <string_view>

namespace Zappy::Server {

/**
 * @class GuiProtocol
 * @brief Formate les messages du protocole serveur->GUI
 */
class GuiProtocol {
public:
    /**
     * @brief Code numerique de la direction (1=N, 2=E, 3=S, 4=W)
     */
    [[nodiscard]] static int getDirectionCode(Direction direction) noexcept;

    [[nodiscard]] static std::string formatMsz(int width, int height);
    [[nodiscard]] static std::string formatBct(int x, int y, const Tile &tile);
    [[nodiscard]] static std::string formatTna(std::string_view team);
    [[nodiscard]] static std::string formatSgt(int frequency);

    [[nodiscard]] static std::string formatPnw(int playerId,
        const Player &player, int level, std::string_view team);
    [[nodiscard]] static std::string formatPpo(int playerId,
        const Player &player);
    [[nodiscard]] static std::string formatPlv(int playerId, int level);
    [[nodiscard]] static std::string formatPin(int playerId, const Player &player, const Tile &tile);
};

} // namespace Zappy::Server

#endif
