/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** test_player
*/

#include <criterion/criterion.h>
#include "Player.hpp"

using Zappy::Server::Direction;
using Zappy::Server::Player;

Test(player, forward_vers_le_nord_diminue_y)
{
    Player player(5, 5, Direction::NORTH);

    player.forward(10, 10);
    cr_assert_eq(player.getX(), 5);
    cr_assert_eq(player.getY(), 4);
}

Test(player, forward_vers_l_est_augmente_x)
{
    Player player(5, 5, Direction::EAST);

    player.forward(10, 10);
    cr_assert_eq(player.getX(), 6);
    cr_assert_eq(player.getY(), 5);
}

Test(player, forward_vers_le_sud_augmente_y)
{
    Player player(5, 5, Direction::SOUTH);

    player.forward(10, 10);
    cr_assert_eq(player.getY(), 6);
}

Test(player, forward_vers_l_ouest_diminue_x)
{
    Player player(5, 5, Direction::WEST);

    player.forward(10, 10);
    cr_assert_eq(player.getX(), 4);
}

Test(player, forward_depuis_le_bord_droit_revient_a_gauche)
{
    Player player(9, 5, Direction::EAST);

    player.forward(10, 10);
    cr_assert_eq(player.getX(), 0);
}

Test(player, forward_depuis_le_bord_haut_revient_en_bas)
{
    Player player(5, 0, Direction::NORTH);

    player.forward(10, 10);
    cr_assert_eq(player.getY(), 9);
}

Test(player, left_puis_right_restaure_la_direction)
{
    Player player(0, 0, Direction::NORTH);

    player.turnLeft();
    player.turnRight();
    cr_assert(player.getDirection() == Direction::NORTH);
}

Test(player, turn_left_tourne_dans_le_sens_antihoraire)
{
    Player player(0, 0, Direction::NORTH);

    player.turnLeft();
    cr_assert(player.getDirection() == Direction::WEST);
}

Test(player, turn_right_tourne_dans_le_sens_horaire)
{
    Player player(0, 0, Direction::NORTH);

    player.turnRight();
    cr_assert(player.getDirection() == Direction::EAST);
}

Test(player, quatre_right_font_un_tour_complet)
{
    Player player(0, 0, Direction::SOUTH);

    for (int i = 0; i < 4; ++i)
        player.turnRight();
    cr_assert(player.getDirection() == Direction::SOUTH);
}
