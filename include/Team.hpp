/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Team
*/

#ifndef ZAPPY_TEAM_HPP_
#define ZAPPY_TEAM_HPP_

#include <string>

namespace Zappy::Server {

/**
 * @class Team
 * @brief Une equipe et le nb de places encore dispo
 */
class Team {
public:
    Team(std::string name, int slots) noexcept;

    [[nodiscard]] const std::string &getName() const noexcept { return _name; }
    [[nodiscard]] int getRemainingSlots() const noexcept { return _slots; }
    [[nodiscard]] bool canAcceptClient() const noexcept { return _slots > 0; }

    /**
     * @brief Prend une place dans l'equipe ; leve si elle est pleine
     */
    void claimSlot();

    void releaseSlot() noexcept;

private:
    std::string _name;
    int _slots;
};

} // namespace Zappy::Server

#endif
