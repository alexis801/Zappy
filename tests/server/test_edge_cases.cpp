/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** test_edge_cases
*/

#include <criterion/criterion.h>
#include "Macros.hpp"
#include "Tile.hpp"

using Zappy::Resource;
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
