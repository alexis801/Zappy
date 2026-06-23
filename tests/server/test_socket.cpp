/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** test_socket
*/

#include <criterion/criterion.h>
#include "Socket.hpp"
#include <array>
#include <stdexcept>
#include <sys/socket.h>
#include <unistd.h>
#include <utility>

using Zappy::Server::Socket;

Test(socket, move_transfere_le_fd)
{
    std::array<int, 2> pair = {};

    cr_assert_eq(::socketpair(AF_UNIX, SOCK_STREAM, 0, pair.data()), 0);
    Socket source(pair[0]);
    int fd = source.getFd();
    Socket moved = std::move(source);

    cr_assert_eq(moved.getFd(), fd);
    cr_assert_not(source.isValid());
    ::close(pair[1]);
}

Test(socket, move_assignment_ferme_lancien)
{
    std::array<int, 2> a = {};
    std::array<int, 2> b = {};

    cr_assert_eq(::socketpair(AF_UNIX, SOCK_STREAM, 0, a.data()), 0);
    cr_assert_eq(::socketpair(AF_UNIX, SOCK_STREAM, 0, b.data()), 0);
    Socket first(a[0]);
    Socket second(b[0]);
    second = std::move(first);

    cr_assert_eq(second.getFd(), a[0]);
    cr_assert_not(first.isValid());
    ::close(a[1]);
    ::close(b[1]);
}

Test(socket, listen_cree_une_socket_valide)
{
    Socket server = Socket::listenOn(48811);

    cr_assert(server.isValid());
}

Test(socket, accept_sans_client_renvoie_invalide)
{
    Socket server = Socket::listenOn(48812);

    cr_assert_not(server.acceptClient().isValid());
}

Test(socket, listen_leve_si_port_indispo)
{
    Socket first = Socket::listenOn(48813);
    bool threw = false;

    try {
        Socket second = Socket::listenOn(48813);
    } catch (const std::runtime_error &) {
        threw = true;
    }
    cr_assert(threw);
}

Test(socket, write_envoie_les_datas)
{
    std::array<int, 2> pair = {};

    cr_assert_eq(::socketpair(AF_UNIX, SOCK_STREAM, 0, pair.data()), 0);
    Socket source(pair[0]);
    cr_assert(source.write("hello\n"));
    char buf[16] = {};
    ssize_t count = ::recv(pair[1], buf, sizeof(buf), 0);
    cr_assert_eq(count, 6);
    cr_assert_str_eq(buf, "hello\n");
    ::close(pair[1]);
}
