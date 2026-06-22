/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** test_gui_protocol
*/

#include <criterion/criterion.h>
#include "GuiProtocol.hpp"
#include <string>

using Zappy::Server::GuiProtocol;

Test(gui_protocol, format_pfk)
{
    std::string message = GuiProtocol::formatPfk(3);

    cr_assert_str_eq(message.c_str(), "pfk #3\n");
}

Test(gui_protocol, format_enw)
{
    std::string message = GuiProtocol::formatEnw(7, 2, 4, 9);

    cr_assert_str_eq(message.c_str(), "enw #7 #2 4 9\n");
}

Test(gui_protocol, format_ebo)
{
    std::string message = GuiProtocol::formatEbo(5);

    cr_assert_str_eq(message.c_str(), "ebo #5\n");
}

Test(gui_protocol, format_edi)
{
    std::string message = GuiProtocol::formatEdi(11);

    cr_assert_str_eq(message.c_str(), "edi #11\n");
}
