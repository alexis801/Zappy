/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** test_egg
*/

#include <criterion/criterion.h>
#include "Egg.hpp"

using Zappy::Server::Egg;

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
