/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** test_branches
*/

#include <criterion/criterion.h>
#include "Player.hpp"

using Zappy::Server::Direction;
using Zappy::Server::Player;
using Zappy::Server::CommandKind;

Test(branches, runMovement_forward_avance)
{
    Player player(2, 2, Direction::NORTH, 1);
    auto outcome = player.executeCommand("Forward", 5, 5);

    cr_assert_eq(outcome.kind, CommandKind::OK);
    cr_assert_eq(player.getY(), 1);
}

Test(branches, runMovement_left_tourne_sur_place)
{
    Player player(2, 2, Direction::NORTH, 1);
    auto outcome = player.executeCommand("Left", 5, 5);

    cr_assert_eq(outcome.kind, CommandKind::OK);
    cr_assert_eq(player.getDirection(), Direction::WEST);
    cr_assert_eq(player.getX(), 2);
    cr_assert_eq(player.getY(), 2);
}

Test(branches, runMovement_right_tourne_sur_place)
{
    Player player(2, 2, Direction::NORTH, 1);
    auto outcome = player.executeCommand("Right", 5, 5);

    cr_assert_eq(outcome.kind, CommandKind::OK);
    cr_assert_eq(player.getDirection(), Direction::EAST);
}

Test(branches, runMovement_commande_inconnue_renvoie_ko)
{
    Player player(2, 2, Direction::NORTH, 1);
    auto outcome = player.executeCommand("Unknown", 5, 5);

    cr_assert_eq(outcome.kind, CommandKind::KO);
}

Test(branches, executeCommand_inventory_renvoie_kind_inventory)
{
    Player player(0, 0, Direction::NORTH, 1);
    auto outcome = player.executeCommand("Inventory", 5, 5);

    cr_assert_eq(outcome.kind, CommandKind::INVENTORY);
    cr_assert_str_neq(outcome.text.c_str(), "");
}

Test(branches, turn_left_quatre_fois_revient_au_depart)
{
    Player player(0, 0, Direction::NORTH, 1);

    player.turnLeft();
    cr_assert_eq(player.getDirection(), Direction::WEST);
    player.turnLeft();
    cr_assert_eq(player.getDirection(), Direction::SOUTH);
    player.turnLeft();
    cr_assert_eq(player.getDirection(), Direction::EAST);
    player.turnLeft();
    cr_assert_eq(player.getDirection(), Direction::NORTH);
}

Test(branches, turn_right_quatre_fois_revient_au_depart)
{
    Player player(0, 0, Direction::NORTH, 1);

    player.turnRight();
    cr_assert_eq(player.getDirection(), Direction::EAST);
    player.turnRight();
    cr_assert_eq(player.getDirection(), Direction::SOUTH);
    player.turnRight();
    cr_assert_eq(player.getDirection(), Direction::WEST);
    player.turnRight();
    cr_assert_eq(player.getDirection(), Direction::NORTH);
}
