/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** test_egg
*/

#include <criterion/criterion.h>
#include "Egg.hpp"
#include "Macros.hpp"
#include "Player.hpp"
#include "Team.hpp"
#include <random>
#include <vector>

using Zappy::LIFE_UNITS_AT_START;
using Zappy::Server::Direction;
using Zappy::Server::Egg;
using Zappy::Server::Player;
using Zappy::Server::Team;

Test(egg, construction_stocke_id_equipe_et_position)
{
    Egg egg(7, "Red", 3, 5);

    cr_assert_eq(egg.getId(), 7);
    cr_assert_str_eq(egg.getTeam().c_str(), "Red");
    cr_assert_eq(egg.getX(), 3);
    cr_assert_eq(egg.getY(), 5);
}

Test(egg, deux_oeufs_distincts_gardent_leurs_valeurs)
{
    Egg first(1, "Blue", 0, 0);
    Egg second(2, "Red", 9, 4);

    cr_assert_eq(first.getId(), 1);
    cr_assert_str_eq(first.getTeam().c_str(), "Blue");
    cr_assert_eq(first.getX(), 0);
    cr_assert_eq(first.getY(), 0);
    cr_assert_eq(second.getId(), 2);
    cr_assert_str_eq(second.getTeam().c_str(), "Red");
    cr_assert_eq(second.getX(), 9);
    cr_assert_eq(second.getY(), 4);
}

Test(egg, fork_ajoute_un_slot_a_equipe)
{
    Team team("Red", 2);
    Player player(4, 6, Direction::NORTH, 1);
    std::vector<Egg> eggs;
    int nextEggId = 0;

    cr_assert_eq(team.getRemainingSlots(), 2);
    cr_assert(player.fork(team, eggs, nextEggId));
    cr_assert_eq(team.getRemainingSlots(), 3);
}

Test(egg, fork_pose_un_oeuf_sur_la_case_du_joueur)
{
    Team team("Blue", 1);
    Player player(2, 8, Direction::EAST, 5);
    std::vector<Egg> eggs;
    int nextEggId = 10;

    cr_assert(player.fork(team, eggs, nextEggId));
    cr_assert_eq(eggs.size(), 1);
    cr_assert_eq(eggs[0].getId(), 10);
    cr_assert_str_eq(eggs[0].getTeam().c_str(), "Blue");
    cr_assert_eq(eggs[0].getX(), 2);
    cr_assert_eq(eggs[0].getY(), 8);
    cr_assert_eq(nextEggId, 11);
}

Test(egg, eclosion_consomme_un_oeuf)
{
    std::vector<Egg> eggs = {Egg(1, "Red", 2, 3), Egg(2, "Red", 5, 7)};
    std::mt19937 rng(42);

    std::optional<Egg> hatched = Egg::pickRandomForTeam(eggs, "Red", rng);

    cr_assert(hatched.has_value());
    cr_assert_eq(eggs.size(), 1);
    cr_assert_str_eq(hatched->getTeam().c_str(), "Red");
}

Test(egg, eclosion_sans_oeuf_disponible)
{
    std::vector<Egg> eggs = {Egg(1, "Blue", 0, 0)};
    std::mt19937 rng(42);

    cr_assert_not(Egg::pickRandomForTeam(eggs, "Red", rng).has_value());
    cr_assert_eq(eggs.size(), 1);
}

Test(egg, hatch_spawn_joueur_sur_case_oeuf)
{
    Egg egg(4, "Red", 6, 9);
    Player player = Player::spawnFromEgg(egg, 12);

    cr_assert_eq(player.getX(), 6);
    cr_assert_eq(player.getY(), 9);
    cr_assert_eq(player.getId(), 12);
    cr_assert_eq(player.getLevel(), 1);
    cr_assert_eq(player.getFood(), LIFE_UNITS_AT_START);
}

Test(egg, eclosion_complete_retire_oeuf_et_spawn)
{
    std::vector<Egg> eggs = {Egg(3, "Blue", 4, 8)};
    std::mt19937 rng(1);
    std::optional<Egg> hatched = Egg::pickRandomForTeam(eggs, "Blue", rng);

    cr_assert(hatched.has_value());
    cr_assert_eq(eggs.size(), 0);
    Player player = Player::spawnFromEgg(*hatched, 7);

    cr_assert_eq(player.getX(), 4);
    cr_assert_eq(player.getY(), 8);
}
