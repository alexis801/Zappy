/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** test_victory
*/

#include <criterion/criterion.h>
#include "Team.hpp"
#include "Victory.hpp"

using Zappy::Server::PlayerSnapshot;
using Zappy::Server::Team;
using Zappy::Server::Victory;

Test(win, count_level8_par_equipe)
{
    std::vector<PlayerSnapshot> players = {
        {"Red", 8, true},
        {"Red", 8, true},
        {"Red", 7, true},
        {"Blue", 8, true},
    };

    cr_assert_eq(Victory::countLevel8ForTeam(players, "Red"), 2);
    cr_assert_eq(Victory::countLevel8ForTeam(players, "Blue"), 1);
    cr_assert_eq(Victory::countLevel8ForTeam(players, "Green"), 0);
}

Test(win, count_level8_ignore_joueurs_morts)
{
    std::vector<PlayerSnapshot> players = {
        {"Red", 8, false},
        {"Red", 8, true},
        {"Red", 8, true},
    };

    cr_assert_eq(Victory::countLevel8ForTeam(players, "Red"), 2);
}

Test(win, detect_victory_seuil_six_joueurs)
{
    std::vector<Team> teams = {Team("Red", 10), Team("Blue", 10)};
    std::vector<PlayerSnapshot> fiveRed;

    for (int i = 0; i < 5; ++i)
        fiveRed.push_back({"Red", 8, true});
    cr_assert_not(Victory::detectWinner(fiveRed, teams).has_value());

    std::vector<PlayerSnapshot> sixRed = fiveRed;

    sixRed.push_back({"Red", 8, true});
    std::optional<std::string> winner = Victory::detectWinner(sixRed, teams);

    cr_assert(winner.has_value());
    cr_assert_str_eq(winner->c_str(), "Red");
}

Test(win, detect_victory_premiere_equipe_enregistree)
{
    std::vector<Team> teams = {Team("Red", 10), Team("Blue", 10)};
    std::vector<PlayerSnapshot> players;

    for (int i = 0; i < 6; ++i)
        players.push_back({"Blue", 8, true});
    for (int i = 0; i < 6; ++i)
        players.push_back({"Red", 8, true});
    std::optional<std::string> winner = Victory::detectWinner(players, teams);

    cr_assert(winner.has_value());
    cr_assert_str_eq(winner->c_str(), "Red");
}
