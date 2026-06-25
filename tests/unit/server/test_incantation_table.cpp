/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** test_incantation_table
*/

#include <criterion/criterion.h>
#include <criterion/parameterized.h>
#include "Macros.hpp"
#include <array>

using Zappy::INCANTATION_TABLE;
using Zappy::IncantationRequirement;
using Zappy::MAX_PLAYER_LEVEL;

struct IncantationCase {
    int sourceLevel;
    int expectedPlayers;
    int expectedLinemate;
    int expectedDeraumere;
    int expectedSibur;
    int expectedMendiane;
    int expectedPhiras;
    int expectedThystame;
};

ParameterizedTestParameters(incantation_table, table_conforme_au_sujet)
{
    static IncantationCase cases[] = {
        {1, 1, 1, 0, 0, 0, 0, 0},
        {2, 2, 1, 1, 1, 0, 0, 0},
        {3, 2, 2, 0, 1, 0, 2, 0},
        {4, 4, 1, 1, 2, 0, 1, 0},
        {5, 4, 1, 2, 1, 3, 0, 0},
        {6, 6, 1, 2, 3, 0, 1, 0},
        {7, 6, 2, 2, 2, 2, 2, 1},
    };
    return cr_make_param_array(IncantationCase, cases,
        sizeof(cases) / sizeof(cases[0]));
}

ParameterizedTest(IncantationCase *c, incantation_table,
    table_conforme_au_sujet)
{
    const IncantationRequirement &req =
        INCANTATION_TABLE[c->sourceLevel - 1];

    cr_assert_eq(req.players, c->expectedPlayers,
        "level %d: %d joueurs au lieu de %d",
        c->sourceLevel, req.players, c->expectedPlayers);
    cr_assert_eq(req.stones[0], c->expectedLinemate);
    cr_assert_eq(req.stones[1], c->expectedDeraumere);
    cr_assert_eq(req.stones[2], c->expectedSibur);
    cr_assert_eq(req.stones[3], c->expectedMendiane);
    cr_assert_eq(req.stones[4], c->expectedPhiras);
    cr_assert_eq(req.stones[5], c->expectedThystame);
}

Test(incantation_table, niveau_max_est_huit)
{
    cr_assert_eq(MAX_PLAYER_LEVEL, 8);
}

Test(incantation_table, taille_table_egale_max_moins_un)
{
    cr_assert_eq(INCANTATION_TABLE.size(),
        static_cast<std::size_t>(MAX_PLAYER_LEVEL - 1));
}
