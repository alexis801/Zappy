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
#include "Clock.hpp"
#include <string>
#include <string_view>
#include <vector>

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
    [[nodiscard]] static std::string formatPex(int playerId);
    [[nodiscard]] static std::string formatPbc(int playerId, std::string_view message);
    [[nodiscard]] static std::string formatPic(int x, int y, int level,
        const std::vector<int> &playerIds);
    [[nodiscard]] static std::string formatPie(int x, int y, bool result);
    [[nodiscard]] static std::string formatPfk(int playerId);
    [[nodiscard]] static std::string formatPdr(int playerId, int resource);
    [[nodiscard]] static std::string formatPgt(int playerId, int resource);
    [[nodiscard]] static std::string formatPdi(int playerId);

    [[nodiscard]] static std::string formatEnw(int eggId, int playerId,
        int x, int y);
    [[nodiscard]] static std::string formatEbo(int eggId);
    [[nodiscard]] static std::string formatEdi(int eggId);

    [[nodiscard]] static std::string formatSst(Clock &clock, int frequency);
    [[nodiscard]] static std::string formatSeg(std::string_view team);
    [[nodiscard]] static std::string formatSmg(std::string_view message);
    [[nodiscard]] static std::string formatSuc();
    [[nodiscard]] static std::string formatSbp();
};

} // namespace Zappy::Server

#endif
