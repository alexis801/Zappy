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
