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

Test(command_parser, look_coute_7)
{
    cr_assert_eq(CommandParser::getCommandCost("Look").value(),
        Zappy::action_cost::LOOK);
}

Test(command_parser, broadcast_coute_7)
{
    cr_assert_eq(CommandParser::getCommandCost("Broadcast").value(),
        Zappy::action_cost::BROADCAST);
}

Test(command_parser, fork_coute_42)
{
    cr_assert_eq(CommandParser::getCommandCost("Fork").value(),
        Zappy::action_cost::FORK);
}

Test(command_parser, eject_coute_7)
{
    cr_assert_eq(CommandParser::getCommandCost("Eject").value(),
        Zappy::action_cost::EJECT);
}

Test(command_parser, take_coute_7)
{
    cr_assert_eq(CommandParser::getCommandCost("Take").value(),
        Zappy::action_cost::TAKE);
}

Test(command_parser, set_coute_7)
{
    cr_assert_eq(CommandParser::getCommandCost("Set").value(),
        Zappy::action_cost::SET);
}

Test(command_parser, incantation_coute_300)
{
    cr_assert_eq(CommandParser::getCommandCost("Incantation").value(),
        Zappy::action_cost::INCANTATION);
}

Test(command_parser, connect_nbr_present_dans_la_table)
{
    cr_assert(CommandParser::getCommandCost("Connect_nbr").has_value());
}

Test(command_parser, commande_inconnue)
{
    cr_assert_not(CommandParser::getCommandCost("Unknown").has_value());
}

Test(command_parser, commande_vide)
{
    cr_assert_not(CommandParser::getCommandCost("").has_value());
}

Test(command_parser, take_avec_argument_garde_le_meme_cout)
{
    cr_assert_eq(CommandParser::getCommandCost("Take food").value(),
        Zappy::action_cost::TAKE);
}

Test(command_parser, set_avec_argument_garde_le_meme_cout)
{
    cr_assert_eq(CommandParser::getCommandCost("Set linemate").value(),
        Zappy::action_cost::SET);
}

Test(command_parser, broadcast_avec_message_garde_le_meme_cout)
{
    cr_assert_eq(CommandParser::getCommandCost("Broadcast hello world").value(),
        Zappy::action_cost::BROADCAST);
}

Test(command_parser, casse_sensible)
{
    cr_assert_not(CommandParser::getCommandCost("forward").has_value());
    cr_assert_not(CommandParser::getCommandCost("FORWARD").has_value());
}
