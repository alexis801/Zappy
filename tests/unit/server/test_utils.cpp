/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** test_utils
*/

#include <criterion/criterion.h>
#include "Macros.hpp"
#include <stdexcept>
#include <string>

using Zappy::getFirstCommand;
using Zappy::getRestCommand;
using Zappy::Resource;
using Zappy::searchResource;

Test(utils, search_food)
{
    cr_assert_eq(searchResource("food"), Resource::FOOD);
}

Test(utils, search_chaque_pierre)
{
    cr_assert_eq(searchResource("linemate"), Resource::LINEMATE);
    cr_assert_eq(searchResource("deraumere"), Resource::DERAUMERE);
    cr_assert_eq(searchResource("sibur"), Resource::SIBUR);
    cr_assert_eq(searchResource("mendiane"), Resource::MENDIANE);
    cr_assert_eq(searchResource("phiras"), Resource::PHIRAS);
    cr_assert_eq(searchResource("thystame"), Resource::THYSTAME);
}

Test(utils, search_ressource_inconnue_jette)
{
    cr_assert_throw(searchResource("unknown"), std::invalid_argument);
}

Test(utils, search_chaine_vide_jette)
{
    cr_assert_throw(searchResource(""), std::invalid_argument);
}

Test(utils, rest_command_renvoie_largument)
{
    std::string rest = getRestCommand("Take food");

    cr_assert_str_eq(rest.c_str(), "food");
}

Test(utils, rest_command_sans_argument_renvoie_vide)
{
    std::string rest = getRestCommand("Forward");

    cr_assert_str_eq(rest.c_str(), "");
}

Test(utils, rest_command_avec_plusieurs_arguments)
{
    std::string rest = getRestCommand("Broadcast hello world");

    cr_assert_str_eq(rest.c_str(), "hello world");
}

Test(utils, first_command_renvoie_le_nom)
{
    std::string first = getFirstCommand("Take food");

    cr_assert_str_eq(first.c_str(), "Take");
}

Test(utils, first_command_sans_argument)
{
    std::string first = getFirstCommand("Forward");

    cr_assert_str_eq(first.c_str(), "Forward");
}

Test(utils, first_command_chaine_vide)
{
    std::string first = getFirstCommand("");

    cr_assert_str_eq(first.c_str(), "");
}
