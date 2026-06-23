/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** test_victory
*/

#include <criterion/criterion.h>
#include "Victory.hpp"

using Zappy::Server::PlayerSnapshot;
using Zappy::Server::Victory;

Test(win, count_level8_par_equipe)
{
    std::vector<PlayerSnapshot> players = {
        {"Red", 8, true},
        {"Red", 8, true},
        {"Red", 7, true},
        {"Blue", 8, true},
    };

    cr_assert_eq(Victory::countLevel8ForTeam(players, "Red"), 2);
    cr_assert_eq(Victory::countLevel8ForTeam(players, "Blue"), 1);
    cr_assert_eq(Victory::countLevel8ForTeam(players, "Green"), 0);
}

Test(win, count_level8_ignore_joueurs_morts)
{
    std::vector<PlayerSnapshot> players = {
        {"Red", 8, false},
        {"Red", 8, true},
        {"Red", 8, true},
    };

    cr_assert_eq(Victory::countLevel8ForTeam(players, "Red"), 2);
}
