/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** test_edge_cases
*/

#include <criterion/criterion.h>
#include "Macros.hpp"
#include "Map.hpp"
#include "Player.hpp"
#include "Team.hpp"
#include "Tile.hpp"

using Zappy::LIFE_UNITS_AT_START;
using Zappy::Resource;
using Zappy::Server::Direction;
using Zappy::Server::Map;
using Zappy::Server::Player;
using Zappy::Server::Team;
using Zappy::Server::Tile;

Test(tile_edge, vide_a_la_creation)
{
    Tile tile;

    cr_assert(tile.isEmpty());
    cr_assert_eq(tile.total(), 0);
}

Test(tile_edge, total_compte_toutes_les_ressources)
{
    Tile tile;

    tile.add(Resource::FOOD, 2);
    tile.add(Resource::LINEMATE, 1);
    tile.add(Resource::THYSTAME, 3);
    cr_assert_eq(tile.total(), 6);
    cr_assert_not(tile.isEmpty());
}

Test(tile_edge, remove_jamais_negatif)
{
    Tile tile;

    tile.add(Resource::FOOD, 1);
    tile.remove(Resource::FOOD, 5);
    cr_assert_eq(tile.get(Resource::FOOD), 0);
}

Test(tile_edge, redevient_vide_apres_remove)
{
    Tile tile;

    tile.add(Resource::SIBUR, 2);
    cr_assert_not(tile.isEmpty());
    tile.remove(Resource::SIBUR, 2);
    cr_assert(tile.isEmpty());
}

Test(player_edge, niveau_initial_a_un)
{
    Player player(0, 0, Direction::NORTH, 1);

    cr_assert_eq(player.getLevel(), 1);
}

Test(player_edge, set_level_modifie_la_valeur)
{
    Player player(0, 0, Direction::NORTH, 1);

    player.setLevel(5);
    cr_assert_eq(player.getLevel(), 5);
}

Test(player_edge, gele_par_defaut_a_faux)
{
    Player player(0, 0, Direction::NORTH, 1);

    cr_assert_not(player.isFrozen());
}

Test(player_edge, set_frozen_change_etat)
{
    Player player(0, 0, Direction::NORTH, 1);

    player.setFrozen(true);
    cr_assert(player.isFrozen());
    player.setFrozen(false);
    cr_assert_not(player.isFrozen());
}

Test(player_edge, nourriture_initiale)
{
    Player player(0, 0, Direction::NORTH, 1);

    cr_assert_eq(player.getFood(), LIFE_UNITS_AT_START);
    cr_assert(player.isAlive());
}

Test(player_edge, mange_seulement_si_food_sur_case)
{
    Player player(0, 0, Direction::NORTH, 1);
    Tile empty;

    cr_assert_not(player.eat(empty));
    Tile tile;
    tile.add(Resource::FOOD, 1);
    cr_assert(player.eat(tile));
    cr_assert_eq(tile.get(Resource::FOOD), 0);
}

Test(map_edge, taille_correcte)
{
    Map map(5, 3);

    cr_assert_eq(map.getWidth(), 5);
    cr_assert_eq(map.getHeight(), 3);
}

Test(map_edge, toutes_les_cases_vides_a_la_creation)
{
    Map map(2, 2);

    for (int y = 0; y < 2; ++y)
        for (int x = 0; x < 2; ++x)
            cr_assert(map.at(x, y).isEmpty());
}

Test(team_edge, place_libre_a_la_creation)
{
    Team team("red", 3);

    cr_assert_eq(team.getRemainingSlots(), 3);
    cr_assert(team.canAcceptClient());
}

Test(team_edge, refuse_quand_plus_de_place)
{
    Team team("red", 1);

    team.claimSlot();
    cr_assert_eq(team.getRemainingSlots(), 0);
    cr_assert_not(team.canAcceptClient());
}

Test(team_edge, release_redonne_une_place)
{
    Team team("red", 1);

    team.claimSlot();
    team.releaseSlot();
    cr_assert_eq(team.getRemainingSlots(), 1);
    cr_assert(team.canAcceptClient());
}

Test(team_edge, add_slot_ajoute_une_place)
{
    Team team("red", 1);

    team.addSlot();
    cr_assert_eq(team.getRemainingSlots(), 2);
}
