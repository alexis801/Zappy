/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** test_player_life
*/

#include <criterion/criterion.h>
#include "Macros.hpp"
#include "Player.hpp"
#include "Tile.hpp"

using Zappy::LIFE_UNITS_AT_START;
using Zappy::Resource;
using Zappy::Server::Direction;
using Zappy::Server::Player;
using Zappy::Server::Tile;

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

Test(life, countdown_consomme_la_nourriture)
{
    Player player(0, 0, Direction::NORTH);

    player.consumeFood(Zappy::TIME_UNITS_PER_FOOD);
    cr_assert_eq(player.getFood(), LIFE_UNITS_AT_START - 1);
    cr_assert(player.isAlive());
}

Test(life, mort_apres_1260_unites_de_temps)
{
    Player player(0, 0, Direction::NORTH);
    int totalLife = LIFE_UNITS_AT_START * Zappy::TIME_UNITS_PER_FOOD;

    player.consumeFood(totalLife);
    cr_assert_eq(player.getFood(), 0);
    cr_assert_not(player.isAlive());
}

Test(life, eat_ajoute_une_nourriture)
{
    Player player(0, 0, Direction::NORTH);
    Tile tile;

    tile.add(Resource::FOOD, 1);
    cr_assert(player.eat(tile));
    cr_assert_eq(player.getFood(), LIFE_UNITS_AT_START + 1);
    cr_assert_eq(tile.get(Resource::FOOD), 0);
}

Test(life, eat_echoue_sans_nourriture_sur_la_case)
{
    Player player(0, 0, Direction::NORTH);
    Tile tile;

    cr_assert_not(player.eat(tile));
    cr_assert_eq(player.getFood(), LIFE_UNITS_AT_START);
}
