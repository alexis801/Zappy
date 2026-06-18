/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** test_command_parser
*/

#include <criterion/criterion.h>
#include "CommandParser.hpp"
#include "Macros.hpp"

using Zappy::Server::CommandParser;

Test(command_parser, forward_coute_7)
{
    cr_assert_eq(CommandParser::getCommandCost("Forward").value(),
        Zappy::action_cost::FORWARD);
}

Test(command_parser, left_coute_7)
{
    cr_assert_eq(CommandParser::getCommandCost("Left").value(),
        Zappy::action_cost::LEFT);
}

Test(command_parser, right_coute_7)
{
    cr_assert_eq(CommandParser::getCommandCost("Right").value(),
        Zappy::action_cost::RIGHT);
}

Test(command_parser, inventory_coute_1)
{
    cr_assert_eq(CommandParser::getCommandCost("Inventory").value(),
        Zappy::action_cost::INVENTORY);
}

Test(command_parser, commande_inconnue)
{
    cr_assert_not(CommandParser::getCommandCost("Look").has_value());
}
