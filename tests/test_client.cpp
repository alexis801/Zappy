/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** test_client
*/

#include <criterion/criterion.h>
#include "Client.hpp"
#include "Socket.hpp"
#include <array>
#include <sys/socket.h>
#include <unistd.h>

using Zappy::Server::Client;
using Zappy::Server::Socket;

namespace {

// Client avec une socket invalide : on ne teste que le tampon de lecture.
Client makeClient()
{
    return Client(Socket());
}

} // namespace

Test(client, ligne_complete)
{
    Client client = makeClient();

    client.feed("bonjour\n");
    cr_assert(client.popLine().value() == "bonjour");
}

Test(client, pas_de_ligne_sans_newline)
{
    Client client = makeClient();

    client.feed("salut");
    cr_assert_not(client.popLine().has_value());
}

Test(client, plusieurs_lignes_dun_coup)
{
    Client client = makeClient();

    client.feed("a\nb\nc\n");
    cr_assert(client.popLine().value() == "a");
    cr_assert(client.popLine().value() == "b");
    cr_assert(client.popLine().value() == "c");
    cr_assert_not(client.popLine().has_value());
}

Test(client, ligne_en_deux_morceaux)
{
    Client client = makeClient();

    client.feed("hel");
    cr_assert_not(client.popLine().has_value());
    client.feed("lo\n");
    cr_assert(client.popLine().value() == "hello");
}

Test(client, garde_le_reste_apres_la_ligne)
{
    Client client = makeClient();

    client.feed("un\nde");
    cr_assert(client.popLine().value() == "un");
    cr_assert_not(client.popLine().has_value());
    client.feed("ux\n");
    cr_assert(client.popLine().value() == "deux");
}

Test(client, recoit_les_donnees_du_socket)
{
    std::array<int, 2> pair = {};

    cr_assert_eq(::socketpair(AF_UNIX, SOCK_STREAM, 0, pair.data()), 0);
    Client client{Socket(pair[0])};
    cr_assert_eq(::write(pair[1], "ping\n", 5), 5);
    cr_assert(client.receive());
    cr_assert(client.popLine().value() == "ping");
    ::close(pair[1]);
}

Test(client, receive_faux_si_deconnecte)
{
    std::array<int, 2> pair = {};

    cr_assert_eq(::socketpair(AF_UNIX, SOCK_STREAM, 0, pair.data()), 0);
    Client client{Socket(pair[0])};
    ::close(pair[1]);
    cr_assert_not(client.receive());
}
