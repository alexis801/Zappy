/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** test_player
*/

#include <criterion/criterion.h>
#include <string>
#include "Macros.hpp"
#include "Player.hpp"

using Zappy::LIFE_UNITS_AT_START;
using Zappy::Resource;
using Zappy::Server::CommandKind;
using Zappy::Server::CommandOutcome;
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

Test(player, spawn_random_reste_dans_la_carte)
{
    for (int i = 0; i < 100; ++i) {
        Player player = Player::spawnRandom(7, 3);

        cr_assert(player.getX() >= 0 && player.getX() < 7);
        cr_assert(player.getY() >= 0 && player.getY() < 3);
    }
}

Test(player, inventaire_demarre_avec_10_food)
{
    Player player(0, 0, Direction::NORTH);

    cr_assert_eq(player.get(Resource::FOOD), LIFE_UNITS_AT_START);
    cr_assert_eq(player.get(Resource::LINEMATE), 0);
}

Test(player, format_inventory_au_spawn)
{
    Player player(0, 0, Direction::NORTH);
    std::string inventory = player.formatInventory();

    cr_assert_str_eq(inventory.c_str(),
        "[food 10, linemate 0, deraumere 0, sibur 0, mendiane 0, "
        "phiras 0, thystame 0]");
}

Test(player, spawn_random_demarre_avec_10_food)
{
    Player player = Player::spawnRandom(5, 5);

    cr_assert_eq(player.get(Resource::FOOD), LIFE_UNITS_AT_START);
}

Test(player, execute_command_inventory)
{
    Player player(0, 0, Direction::NORTH);
    CommandOutcome outcome = player.executeCommand("Inventory", 10, 10);

    cr_assert(outcome.kind == CommandKind::INVENTORY);
    cr_assert_str_eq(outcome.text.c_str(),
        "[food 10, linemate 0, deraumere 0, sibur 0, mendiane 0, "
        "phiras 0, thystame 0]");
}

Test(player, execute_command_forward_ok)
{
    Player player(5, 5, Direction::NORTH);
    CommandOutcome outcome = player.executeCommand("Forward", 10, 10);

    cr_assert(outcome.kind == CommandKind::OK);
    cr_assert_eq(player.getY(), 4);
}

Test(player, execute_command_look)
{
    Player player(0, 0, Direction::NORTH);
    CommandOutcome outcome = player.executeCommand("Look", 10, 10);

    cr_assert(outcome.kind == CommandKind::LOOK);
}

Test(player, execute_command_inconnue_ko)
{
    Player player(0, 0, Direction::NORTH);
    CommandOutcome outcome = player.executeCommand("Incantation", 10, 10);

    cr_assert(outcome.kind == CommandKind::KO);
}
