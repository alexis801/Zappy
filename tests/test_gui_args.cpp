/*
** EPITECH PROJECT, 2026
** zappy_gotenku
** File description:
** test_gui_args.cpp
*/

#include <criterion/criterion.h>
#include "gui/Args.hpp"
#include <string>
#include <vector>
#include <stdexcept>

using Zappy::Gui::Args;

namespace {

Args buildArgs(std::vector<std::string> tokens)
{
    std::vector<char *> argv;

    for (auto &token : tokens)
        argv.push_back(token.data());
    argv.push_back(nullptr);
    return Args(static_cast<int>(tokens.size()), argv.data());
}

bool throwsOn(std::vector<std::string> tokens)
{
    try {
        buildArgs(std::move(tokens));
    } catch (const std::invalid_argument &) {
        return true;
    }
    return false;
}

} // namespace

Test(gui_args, valeurs_de_base_ok)
{
    Args args = buildArgs({"zappy_gui", "-p", "4242", "-h", "localhost"});

    cr_assert_eq(args.getPort(), 4242);
    cr_assert(args.getHost() == "localhost");
}

Test(gui_args, host_par_defaut_localhost)
{
    Args args = buildArgs({"zappy_gui", "-p", "4242"});

    cr_assert(args.getHost() == "localhost");
}

Test(gui_args, host_custom)
{
    Args args = buildArgs({"zappy_gui", "-p", "1", "-h", "10.0.0.1"});

    cr_assert(args.getHost() == "10.0.0.1");
}

Test(gui_args, option_inconnue)
{
    cr_assert(throwsOn({"zappy_gui", "-nonono"}));
}

Test(gui_args, port_pas_numerique)
{
    cr_assert(throwsOn({"zappy_gui", "-p", "zapzap"}));
}

Test(gui_args, port_avec_dechet)
{
    cr_assert(throwsOn({"zappy_gui", "-p", "42zapzap"}));
}

Test(gui_args, port_negatif)
{
    cr_assert(throwsOn({"zappy_gui", "-p", "-5"}));
}

Test(gui_args, flag_p_sans_valeur)
{
    cr_assert(throwsOn({"zappy_gui", "-p"}));
}

Test(gui_args, flag_h_sans_valeur)
{
    cr_assert(throwsOn({"zappy_gui", "-p", "4242", "-h"}));
}

Test(gui_args, port_manquant)
{
    cr_assert(throwsOn({"zappy_gui", "-h", "localhost"}));
}

Test(gui_args, aucun_argument)
{
    cr_assert(throwsOn({"zappy_gui"}));
}