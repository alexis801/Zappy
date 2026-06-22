/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Team
*/

#include "Team.hpp"
#include <stdexcept>
#include <utility>

namespace Zappy::Server {

Team::Team(std::string name, int slots) noexcept
    : _name(std::move(name)), _slots(slots)
{
}

void Team::claimSlot()
{
    if (!canAcceptClient())
        throw std::runtime_error("Team '" + _name + "' is full");
    --_slots;
}

void Team::releaseSlot() noexcept
{
    ++_slots;
}

void Team::addSlot() noexcept
{
    ++_slots;
}

} // namespace Zappy::Server
