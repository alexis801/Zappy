/*
** EPITECH PROJECT, 2026
** zappy_gotenku
** File description:
** test_gui_socket.cpp
*/

#include <criterion/criterion.h>
#include "gui/Socket.hpp"
#include <array>
#include <sys/socket.h>
#include <unistd.h>
#include <utility>

using Zappy::Gui::Socket;

Test(gui_socket, socket_par_defaut_invalide)
{
    Socket socket;

    cr_assert_not(socket.isValid());
}

Test(gui_socket, move_transfere_le_fd)
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

Test(gui_socket, move_assignment_ferme_lancien)
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