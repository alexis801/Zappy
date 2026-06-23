/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** test_args
*/

#include <criterion/criterion.h>
#include "Args.hpp"
#include "Macros.hpp"
#include <stdexcept>
#include <string>
#include <vector>

using Zappy::Server::Args;

namespace {

// Construit un Args a partir d'une liste de token
Args buildArgs(std::vector<std::string> tokens)
{
    std::vector<char *> argv;

    for (auto &token : tokens)
        argv.push_back(token.data());
    argv.push_back(nullptr);
    return Args(static_cast<int>(tokens.size()), argv.data());
}

// ret true si construire Args avec ces tokens leve invalid_arg.
bool throwsOn(std::vector<std::string> tokens)
{
    try {
        buildArgs(std::move(tokens));
    } catch (const std::invalid_argument &) {
        return true;
    }
    return false;
}

// true si isHelpRequested detecte -help/--help
bool helpRequestedFor(std::vector<std::string> tokens)
{
    std::vector<char *> argv;

    for (auto &token : tokens)
        argv.push_back(token.data());
    argv.push_back(nullptr);
    return Args::isHelpRequested(static_cast<int>(tokens.size()), argv.data());
}

} // namespace

Test(args, valeurs_de_base_ok)
{
    Args args = buildArgs({"zappy_server", "-p", "4242", "-x", "10",
        "-y", "20", "-n", "red", "blue", "-c", "5"});

    cr_assert_eq(args.getPort(), 4242);
    cr_assert_eq(args.getWidth(), 10);
    cr_assert_eq(args.getHeight(), 20);
    cr_assert_eq(args.getClientsPerTeam(), 5);
}

Test(args, frequence_par_def_100)
{
    Args args = buildArgs({"zappy_server", "-p", "1", "-x", "1",
        "-y", "1", "-n", "a", "-c", "1"});

    cr_assert_eq(args.getFrequency(), 100);
}

Test(args, frequence_custom)
{
    Args args = buildArgs({"zappy_server", "-p", "1", "-x", "1",
        "-y", "1", "-n", "a", "-c", "1", "-f", "50"});

    cr_assert_eq(args.getFrequency(), 50);
}

Test(args, plusieurs_equipes)
{
    Args args = buildArgs({"zappy_server", "-p", "1", "-x", "1",
        "-y", "1", "-n", "red", "blue", "green", "-c", "1"});

    cr_assert(args.getTeams().size() == 3);
    cr_assert(args.getTeams()[0] == "red");
    cr_assert(args.getTeams()[2] == "green");
}

Test(args, valeurs_par_defaut_sans_args)
{
    Args args = buildArgs({"zappy_server"});

    cr_assert_eq(args.getPort(), Zappy::DEFAULT_PORT);
    cr_assert_eq(args.getWidth(), Zappy::DEFAULT_WIDTH);
    cr_assert_eq(args.getHeight(), Zappy::DEFAULT_HEIGHT);
    cr_assert_eq(args.getClientsPerTeam(), Zappy::DEFAULT_CLIENTS_PER_TEAM);
    cr_assert_eq(args.getFrequency(), Zappy::DEFAULT_FREQUENCY);
    cr_assert_eq(args.getTeams().size(), 1u);
    cr_assert(args.getTeams()[0] == Zappy::DEFAULT_TEAM_NAME);
}

Test(args, port_omis_prend_le_defaut)
{
    Args args = buildArgs({"zappy_server", "-x", "5", "-y", "5",
        "-n", "a", "-c", "2"});

    cr_assert_eq(args.getPort(), Zappy::DEFAULT_PORT);
}

Test(args, n_remplace_l_equipe_par_defaut)
{
    Args args = buildArgs({"zappy_server", "-n", "red", "blue"});

    cr_assert_eq(args.getTeams().size(), 2u);
    cr_assert(args.getTeams()[0] == "red");
    cr_assert(args.getTeams()[1] == "blue");
}

Test(args, n_sans_nom_est_refuse)
{
    cr_assert(throwsOn({"zappy_server", "-n", "-p", "4242"}));
}

Test(args, refuse_equipes_en_double)
{
    cr_assert(throwsOn({"zappy_server", "-p", "1", "-x", "1",
        "-y", "1", "-n", "red", "red", "-c", "1"}));
    cr_assert(throwsOn({"zappy_server", "-p", "1", "-x", "1",
        "-y", "1", "-n", "red", "blue", "red", "-c", "1"}));
}

Test(args, option_inconnue)
{
    cr_assert(throwsOn({"zappy_server", "-z"}));
}

Test(args, valeur_pas_numerique)
{
    cr_assert(throwsOn({"zappy_server", "-p", "abc", "-x", "1",
        "-y", "1", "-n", "a", "-c", "1"}));
}

Test(args, valeur_avec_dechet)
{
    cr_assert(throwsOn({"zappy_server", "-p", "10abc", "-x", "1",
        "-y", "1", "-n", "a", "-c", "1"}));
}

Test(args, valeur_negative)
{
    cr_assert(throwsOn({"zappy_server", "-p", "-5", "-x", "1",
        "-y", "1", "-n", "a", "-c", "1"}));
}

Test(args, flag_sans_valeur)
{
    cr_assert(throwsOn({"zappy_server", "-p"}));
}

Test(args, usage_non_vide)
{
    cr_assert(!Args::getUsage().empty());
}

Test(args, detecte_le_flag_help)
{
    cr_assert(helpRequestedFor({"zappy_server", "--help"}));
    cr_assert(helpRequestedFor({"zappy_server", "-help"}));
}

Test(args, pas_de_flag_help)
{
    cr_assert_not(helpRequestedFor({"zappy_server", "-p", "4242"}));
}
