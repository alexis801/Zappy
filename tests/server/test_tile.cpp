/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** test_tile
*/

#include <criterion/criterion.h>
#include "Tile.hpp"

using Zappy::Server::Tile;
using Zappy::Resource;

Test(tile, case_vide_a_zero_partout)
{
    Tile tile;

    cr_assert_eq(tile.get(Resource::FOOD), 0);
    cr_assert_eq(tile.get(Resource::LINEMATE), 0);
    cr_assert_eq(tile.get(Resource::THYSTAME), 0);
}

Test(tile, add_augmente_la_quantite)
{
    Tile tile;

    tile.add(Resource::FOOD, 3);
    cr_assert_eq(tile.get(Resource::FOOD), 3);
    tile.add(Resource::FOOD, 2);
    cr_assert_eq(tile.get(Resource::FOOD), 5);
}

Test(tile, add_est_independant_par_ressource)
{
    Tile tile;

    tile.add(Resource::LINEMATE, 4);
    cr_assert_eq(tile.get(Resource::LINEMATE), 4);
    cr_assert_eq(tile.get(Resource::FOOD), 0);
    cr_assert_eq(tile.get(Resource::SIBUR), 0);
}

Test(tile, remove_diminue_la_quantite)
{
    Tile tile;

    tile.add(Resource::FOOD, 5);
    tile.remove(Resource::FOOD, 2);
    cr_assert_eq(tile.get(Resource::FOOD), 3);
}

Test(tile, remove_sous_zero_reste_a_zero)
{
    Tile tile;

    tile.add(Resource::SIBUR, 2);
    tile.remove(Resource::SIBUR, 5);
    cr_assert_eq(tile.get(Resource::SIBUR), 0);
}

Test(tile, remove_sur_case_vide_reste_a_zero)
{
    Tile tile;

    tile.remove(Resource::PHIRAS, 1);
    cr_assert_eq(tile.get(Resource::PHIRAS), 0);
}

Test(tile, isEmpty_vrai_sur_case_vide)
{
    Tile tile;

    cr_assert(tile.isEmpty());
}

Test(tile, isEmpty_faux_si_une_ressource)
{
    Tile tile;

    tile.add(Resource::FOOD, 1);
    cr_assert_not(tile.isEmpty());
}

Test(tile, total_somme_toutes_ressources)
{
    Tile tile;

    cr_assert_eq(tile.total(), 0);
    tile.add(Resource::FOOD, 3);
    tile.add(Resource::LINEMATE, 2);
    cr_assert_eq(tile.total(), 5);
    tile.remove(Resource::FOOD, 1);
    cr_assert_eq(tile.total(), 4);
}
