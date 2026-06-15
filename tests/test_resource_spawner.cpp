/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** test_resource_spawner
*/

#include <criterion/criterion.h>
#include "ResourceSpawner.hpp"

using Zappy::Resource;
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
