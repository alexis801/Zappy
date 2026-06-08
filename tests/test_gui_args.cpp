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

using Zappy::Gui::Args;

namespace {

    bool helpRequestedFor(std::vector<std::string> tokens)
{
    std::vector<char *> argv;

    for (auto &token : tokens)
        argv.push_back(token.data());
    argv.push_back(nullptr);
    return Args::isHelp(static_cast<int>(tokens.size()), argv.data());
}

} // namespace

Test(gui_args, usage_non_vide)
{
    cr_assert(!Args::getUsage().empty());
}

Test(gui_args, detecte_le_flag_help)
{
    cr_assert(helpRequestedFor({"zappy_gui", "--help"}));
    cr_assert(helpRequestedFor({"zappy_gui", "-help"}));
}

Test(gui_args, h_nest_pas_help)
{
    cr_assert_not(helpRequestedFor({"zappy_gui", "-h", "localhost"}));
}