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
