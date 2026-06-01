/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** test_team
*/

#include <criterion/criterion.h>
#include "Team.hpp"
#include <stdexcept>

using Zappy::Server::Team;

Test(team, contient_son_nom_et_ses_places)
{
    Team team("rouge", 5);

    cr_assert(team.getName() == "rouge");
    cr_assert_eq(team.getRemainingSlots(), 5);
    cr_assert(team.canAcceptClient());
}

Test(team, prendre_une_place_decremente)
{
    Team team("a", 3);

    team.claimSlot();
    cr_assert_eq(team.getRemainingSlots(), 2);
    team.claimSlot();
    team.claimSlot();
    cr_assert_eq(team.getRemainingSlots(), 0);
    cr_assert_not(team.canAcceptClient());
}

Test(team, equipe_pleine_leve_exception)
{
    Team team("solo", 1);
    bool threw = false;

    team.claimSlot();
    try {
        team.claimSlot();
    } catch (const std::runtime_error &) {
        threw = true;
    }
    cr_assert(threw);
}
