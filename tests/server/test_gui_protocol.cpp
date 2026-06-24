/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** test_gui_protocol
*/

#include <criterion/criterion.h>
#include "GuiProtocol.hpp"
#include "Player.hpp"
#include "Tile.hpp"
#include "Clock.hpp"
#include "Macros.hpp"
#include <string>

using Zappy::DEFAULT_FREQUENCY;
using Zappy::Resource;
using Zappy::Server::Clock;
using Zappy::Server::Direction;
using Zappy::Server::GuiProtocol;
using Zappy::Server::Player;
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

Test(gui_protocol, format_pnw)
{
    Player player(3, 5, Direction::EAST, 1);
    std::string msg = GuiProtocol::formatPnw(1, player, 1, "red");

    cr_assert_str_eq(msg.c_str(), "pnw #1 3 5 2 1 red\n");
}

Test(gui_protocol, format_ppo_north)
{
    Player player(2, 3, Direction::NORTH, 1);
    std::string msg = GuiProtocol::formatPpo(1, player);

    cr_assert_str_eq(msg.c_str(), "ppo #1 2 3 1\n");
}

Test(gui_protocol, format_ppo_east)
{
    Player player(2, 3, Direction::EAST, 1);
    std::string msg = GuiProtocol::formatPpo(1, player);

    cr_assert_str_eq(msg.c_str(), "ppo #1 2 3 2\n");
}

Test(gui_protocol, format_ppo_south)
{
    Player player(2, 3, Direction::SOUTH, 1);
    std::string msg = GuiProtocol::formatPpo(1, player);

    cr_assert_str_eq(msg.c_str(), "ppo #1 2 3 3\n");
}

Test(gui_protocol, format_ppo_west)
{
    Player player(2, 3, Direction::WEST, 1);
    std::string msg = GuiProtocol::formatPpo(1, player);

    cr_assert_str_eq(msg.c_str(), "ppo #1 2 3 4\n");
}

Test(gui_protocol, format_plv)
{
    std::string msg = GuiProtocol::formatPlv(5, 3);

    cr_assert_str_eq(msg.c_str(), "plv #5 3\n");
}

Test(gui_protocol, format_pin_empty_tile)
{
    Player player(4, 7, Direction::NORTH, 2);
    Tile tile;
    std::string msg = GuiProtocol::formatPin(2, player, tile);

    cr_assert_str_eq(msg.c_str(), "pin #2 4 7 0 0 0 0 0 0 0\n");
}

Test(gui_protocol, format_pex)
{
    std::string msg = GuiProtocol::formatPex(8);

    cr_assert_str_eq(msg.c_str(), "pex #8\n");
}

Test(gui_protocol, format_pbc)
{
    std::string msg = GuiProtocol::formatPbc(4, "hello");

    cr_assert_str_eq(msg.c_str(), "pbc #4 hello\n");
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
