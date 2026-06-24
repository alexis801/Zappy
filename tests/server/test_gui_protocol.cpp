/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** test_gui_protocol
*/

#include <criterion/criterion.h>
#include "GuiProtocol.hpp"
#include "Tile.hpp"
#include "Clock.hpp"
#include "Macros.hpp"
#include <string>

using Zappy::DEFAULT_FREQUENCY;
using Zappy::Resource;
using Zappy::Server::Clock;
using Zappy::Server::GuiProtocol;
using Zappy::Server::Tile;

Test(gui_protocol, format_msz)
{
    std::string msg = GuiProtocol::formatMsz(10, 20);

    cr_assert_str_eq(msg.c_str(), "msz 10 20\n");
}

Test(gui_protocol, format_msz_zero_size)
{
    std::string msg = GuiProtocol::formatMsz(0, 0);

    cr_assert_str_eq(msg.c_str(), "msz 0 0\n");
}

Test(gui_protocol, format_bct_empty_tile)
{
    Tile tile;
    std::string msg = GuiProtocol::formatBct(2, 3, tile);

    cr_assert_str_eq(msg.c_str(), "bct 2 3 0 0 0 0 0 0 0\n");
}

Test(gui_protocol, format_bct_with_resources)
{
    Tile tile;

    tile.add(Resource::FOOD, 2);
    tile.add(Resource::LINEMATE, 1);
    tile.add(Resource::THYSTAME, 3);
    std::string msg = GuiProtocol::formatBct(0, 0, tile);

    cr_assert_str_eq(msg.c_str(), "bct 0 0 2 1 0 0 0 0 3\n");
}

Test(gui_protocol, format_tna)
{
    std::string msg = GuiProtocol::formatTna("red");

    cr_assert_str_eq(msg.c_str(), "tna red\n");
}

Test(gui_protocol, format_sgt)
{
    std::string msg = GuiProtocol::formatSgt(100);

    cr_assert_str_eq(msg.c_str(), "sgt 100\n");
}

Test(gui_protocol, format_sst)
{
    Clock clock(DEFAULT_FREQUENCY);
    std::string msg = GuiProtocol::formatSst(clock, 200);

    cr_assert_str_eq(msg.c_str(), "sst 200\n");
}

Test(gui_protocol, format_seg)
{
    std::string msg = GuiProtocol::formatSeg("blue");

    cr_assert_str_eq(msg.c_str(), "seg blue\n");
}

Test(gui_protocol, format_smg)
{
    std::string msg = GuiProtocol::formatSmg("game starts");

    cr_assert_str_eq(msg.c_str(), "smg game starts\n");
}

Test(gui_protocol, format_suc)
{
    std::string msg = GuiProtocol::formatSuc();

    cr_assert_str_eq(msg.c_str(), "suc\n");
}

Test(gui_protocol, format_sbp)
{
    std::string msg = GuiProtocol::formatSbp();

    cr_assert_str_eq(msg.c_str(), "sbp\n");
}

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
