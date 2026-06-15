/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** test_resource_spawner
*/

#include <criterion/criterion.h>
#include "Map.hpp"
#include "ResourceSpawner.hpp"

using Zappy::Resource;
using Zappy::Server::Map;
using Zappy::Server::ResourceSpawner;

Test(res, quantites_cibles_carte_10x10)
{
    cr_assert_eq(
        ResourceSpawner::getTargetQuantity(Resource::FOOD, 10, 10), 50);
    cr_assert_eq(
        ResourceSpawner::getTargetQuantity(Resource::LINEMATE, 10, 10), 30);
    cr_assert_eq(
        ResourceSpawner::getTargetQuantity(Resource::THYSTAME, 10, 10), 5);
}

Test(res, minimum_un_par_ressource)
{
    cr_assert_eq(
        ResourceSpawner::getTargetQuantity(Resource::FOOD, 1, 1), 1);
    cr_assert_eq(
        ResourceSpawner::getTargetQuantity(Resource::THYSTAME, 1, 1), 1);
    cr_assert_eq(
        ResourceSpawner::getTargetQuantity(Resource::SIBUR, 2, 2), 1);
}

Test(res, comptage_carte_vide)
{
    Map map(5, 5);

    cr_assert_eq(ResourceSpawner::countOnMap(Resource::FOOD, map), 0);
    cr_assert_eq(ResourceSpawner::countOnMap(Resource::LINEMATE, map), 0);
}

Test(res, comptage_somme_sur_plusieurs_cases)
{
    Map map(3, 3);

    map.at(0, 0).add(Resource::FOOD, 2);
    map.at(1, 2).add(Resource::FOOD, 3);
    map.at(2, 1).add(Resource::LINEMATE, 4);
    cr_assert_eq(ResourceSpawner::countOnMap(Resource::FOOD, map), 5);
    cr_assert_eq(ResourceSpawner::countOnMap(Resource::LINEMATE, map), 4);
    cr_assert_eq(ResourceSpawner::countOnMap(Resource::SIBUR, map), 0);
}
