/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** test_map
*/

#include <criterion/criterion.h>
#include "Map.hpp"

using Zappy::Server::Map;

Test(map, dimensions)
{
    Map map(10, 20);

    cr_assert_eq(map.getWidth(), 10);
    cr_assert_eq(map.getHeight(), 20);
}

Test(map, dimensions_carre)
{
    Map map(5, 5);

    cr_assert_eq(map.getWidth(), 5);
    cr_assert_eq(map.getHeight(), 5);
}

Test(map, acces_case_par_coordonnees)
{
    Map map(10, 10);

    map.at(2, 3).add(Zappy::Resource::FOOD, 4);
    cr_assert_eq(map.at(2, 3).get(Zappy::Resource::FOOD), 4);
    cr_assert_eq(map.at(0, 0).get(Zappy::Resource::FOOD), 0);
}

Test(map, cases_distinctes)
{
    Map map(3, 3);

    map.at(1, 0).add(Zappy::Resource::LINEMATE, 1);
    map.at(2, 1).add(Zappy::Resource::SIBUR, 2);
    cr_assert_eq(map.at(1, 0).get(Zappy::Resource::LINEMATE), 1);
    cr_assert_eq(map.at(2, 1).get(Zappy::Resource::SIBUR), 2);
    cr_assert_eq(map.at(1, 0).get(Zappy::Resource::SIBUR), 0);
}
