/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** test_player_life
*/

#include <criterion/criterion.h>
#include "Macros.hpp"
#include "Player.hpp"

using Zappy::LIFE_UNITS_AT_START;
using Zappy::Server::Direction;
using Zappy::Server::Player;

Test(life, valeurs_depart_niveau_et_nourriture)
{
    Player player(3, 7, Direction::EAST);

    cr_assert_eq(player.getLevel(), 1);
    cr_assert_eq(player.getFood(), LIFE_UNITS_AT_START);
}

Test(life, spawn_random_conserve_les_valeurs_depart)
{
    Player player = Player::spawnRandom(10, 10);

    cr_assert_eq(player.getLevel(), 1);
    cr_assert_eq(player.getFood(), LIFE_UNITS_AT_START);
}
