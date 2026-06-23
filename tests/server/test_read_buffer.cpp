/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** test_read_buffer
*/

#include <criterion/criterion.h>
#include "ReadBuffer.hpp"

using Zappy::Server::ReadBuffer;

Test(read_buffer, vide_au_depart)
{
    ReadBuffer buffer;

    cr_assert(buffer.isEmpty());
    cr_assert_not(buffer.popLine().has_value());
}

Test(read_buffer, ligne_complete)
{
    ReadBuffer buffer;

    buffer.feed("bonjour\n");
    cr_assert(buffer.popLine().value() == "bonjour");
    cr_assert(buffer.isEmpty());
}

Test(read_buffer, pas_de_ligne_sans_newline)
{
    ReadBuffer buffer;

    buffer.feed("salut");
    cr_assert_not(buffer.popLine().has_value());
    cr_assert_not(buffer.isEmpty());
}

Test(read_buffer, plusieurs_lignes_dun_coup)
{
    ReadBuffer buffer;

    buffer.feed("a\nb\nc\n");
    cr_assert(buffer.popLine().value() == "a");
    cr_assert(buffer.popLine().value() == "b");
    cr_assert(buffer.popLine().value() == "c");
    cr_assert_not(buffer.popLine().has_value());
    cr_assert(buffer.isEmpty());
}

Test(read_buffer, ligne_en_deux_morceaux)
{
    ReadBuffer buffer;

    buffer.feed("hel");
    cr_assert_not(buffer.popLine().has_value());
    buffer.feed("lo\n");
    cr_assert(buffer.popLine().value() == "hello");
}

Test(read_buffer, garde_le_reste_apres_la_ligne)
{
    ReadBuffer buffer;

    buffer.feed("un\nde");
    cr_assert(buffer.popLine().value() == "un");
    cr_assert_not(buffer.popLine().has_value());
    cr_assert_not(buffer.isEmpty());
    buffer.feed("ux\n");
    cr_assert(buffer.popLine().value() == "deux");
}

Test(read_buffer, ligne_vide_acceptee)
{
    ReadBuffer buffer;

    buffer.feed("\n");
    cr_assert(buffer.popLine().value().empty());
}
