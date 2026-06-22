/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** test_look
*/

#include <criterion/criterion.h>
#include <string>
#include "CommandParser.hpp"
#include "Look.hpp"
#include "Macros.hpp"
#include "Map.hpp"
#include "Player.hpp"

using Zappy::Resource;
using Zappy::Server::CommandKind;
using Zappy::Server::Direction;
using Zappy::Server::Map;
using Zappy::Server::Player;
using Zappy::Server::TileCoord;
using Zappy::Server::buildTileContents;
using Zappy::Server::CommandParser;
using Zappy::Server::computeVisibleTileCoords;
using Zappy::Server::formatLookResponse;
using Zappy::Server::performLook;
using Zappy::Server::wrapCoords;

Test(look, wrap_coin_haut_gauche)
{
    TileCoord coord = wrapCoords(-1, -1, 10, 10);

    cr_assert_eq(coord.x, 9);
    cr_assert_eq(coord.y, 9);
}

Test(look, wrap_bord_droit)
{
    TileCoord coord = wrapCoords(10, 5, 10, 10);

    cr_assert_eq(coord.x, 0);
    cr_assert_eq(coord.y, 5);
}

Test(look, wrap_bord_bas)
{
    TileCoord coord = wrapCoords(3, 10, 10, 10);

    cr_assert_eq(coord.x, 3);
    cr_assert_eq(coord.y, 0);
}

Test(look, wrap_bord_gauche)
{
    TileCoord coord = wrapCoords(-1, 4, 10, 10);

    cr_assert_eq(coord.x, 9);
    cr_assert_eq(coord.y, 4);
}

Test(look, niveau_1_quatre_tuiles)
{
    auto tiles = computeVisibleTileCoords(
        5, 5, Direction::NORTH, 1, 10, 10);

    cr_assert_eq(tiles.size(), 4U);
}

Test(look, niveau_2_neuf_tuiles)
{
    auto tiles = computeVisibleTileCoords(
        5, 5, Direction::NORTH, 2, 10, 10);

    cr_assert_eq(tiles.size(), 9U);
}

Test(look, niveau_3_seize_tuiles)
{
    auto tiles = computeVisibleTileCoords(
        5, 5, Direction::NORTH, 3, 10, 10);

    cr_assert_eq(tiles.size(), 16U);
}

Test(look, nord_depuis_origine_devant_en_bas)
{
    auto tiles = computeVisibleTileCoords(
        0, 0, Direction::NORTH, 1, 10, 10);

    cr_assert_eq(tiles[2].x, 0);
    cr_assert_eq(tiles[2].y, 9);
}

Test(look, est_depuis_origine_devant_a_droite)
{
    auto tiles = computeVisibleTileCoords(
        0, 0, Direction::EAST, 1, 10, 10);

    cr_assert_eq(tiles[2].x, 1);
    cr_assert_eq(tiles[2].y, 0);
}

Test(look, sud_depuis_origine_devant_en_bas)
{
    auto tiles = computeVisibleTileCoords(
        0, 0, Direction::SOUTH, 1, 10, 10);

    cr_assert_eq(tiles[2].x, 0);
    cr_assert_eq(tiles[2].y, 1);
}

Test(look, ouest_depuis_origine_devant_a_gauche)
{
    auto tiles = computeVisibleTileCoords(
        0, 0, Direction::WEST, 1, 10, 10);

    cr_assert_eq(tiles[2].x, 9);
    cr_assert_eq(tiles[2].y, 0);
}

Test(look, joueur_seul_niveau_1)
{
    Map map(10, 10);
    Player player(0, 0, Direction::NORTH, 1);

    map.addPlayer(1, 0, 0);
    std::string vision = performLook(player, map);

    cr_assert_str_eq(vision.c_str(), "[player, , , ]");
}

Test(look, food_devant_nord)
{
    Map map(10, 10);
    Player player(0, 0, Direction::NORTH, 1);

    map.addPlayer(1, 0, 0);
    map.at(0, 9).add(Resource::FOOD, 1);
    std::string vision = performLook(player, map);

    cr_assert_str_eq(vision.c_str(), "[player, , food, ]");
}

Test(look, plusieurs_joueurs_meme_tuile)
{
    Map map(10, 10);
    Player player(0, 0, Direction::NORTH, 1);

    map.addPlayer(1, 0, 0);
    map.addPlayer(2, 0, 0);
    std::string vision = performLook(player, map);

    cr_assert_str_eq(vision.c_str(), "[player player, , , ]");
}

Test(look, plusieurs_ressources_meme_tuile)
{
    Map map(10, 10);
    Player player(0, 0, Direction::NORTH, 1);

    map.addPlayer(1, 0, 0);
    map.at(0, 0).add(Resource::FOOD, 2);
    map.at(0, 0).add(Resource::DERAUMERE, 1);
    std::string vision = performLook(player, map);

    cr_assert_str_eq(vision.c_str(),
        "[player food food deraumere, , , ]");
}

Test(look, wrap_coin_nord_ouest)
{
    Map map(10, 10);
    Player player(0, 0, Direction::NORTH, 1);

    map.addPlayer(1, 0, 0);
    map.at(9, 9).add(Resource::LINEMATE, 1);
    std::string vision = performLook(player, map);

    cr_assert_str_eq(vision.c_str(), "[player, linemate, , ]");
}

Test(look, format_tuile_vide)
{
    std::vector<std::vector<std::string>> tiles = {{}, {"food"}, {}};
    std::string formatted = formatLookResponse(tiles);

    cr_assert_str_eq(formatted.c_str(), "[, food, ]");
}

Test(look, parser_coute_7)
{
    cr_assert_eq(CommandParser::getCommandCost("Look").value(),
        Zappy::action_cost::LOOK);
}

Test(look, execute_command_look)
{
    Player player(0, 0, Direction::NORTH, 1);
    auto outcome = player.executeCommand("Look", 10, 10);

    cr_assert(outcome.kind == CommandKind::LOOK);
}
