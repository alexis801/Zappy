/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** test_core_integration
*/

#include <criterion/criterion.h>
#include "Args.hpp"
#include "Core.hpp"
#include <arpa/inet.h>
#include <chrono>
#include <netinet/in.h>
#include <stdexcept>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>

using Zappy::Server::Args;
using Zappy::Server::Core;

namespace {

Args makeArgs(int port)
{
    static char arg0[] = "test";
    static char argp[] = "-p";
    static char argx[] = "-x";
    static char w[] = "5";
    static char argy[] = "-y";
    static char h[] = "5";
    static char argn[] = "-n";
    static char team[] = "red";
    char portStr[16];

    std::snprintf(portStr, sizeof(portStr), "%d", port);
    char *argv[] = {arg0, argp, portStr, argx, w, argy, h, argn, team};
    return Args(9, argv);
}

int connectClient(int port)
{
    int fd = ::socket(AF_INET, SOCK_STREAM, 0);

    if (fd < 0)
        return -1;
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(static_cast<uint16_t>(port));
    ::inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
    if (::connect(fd, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) < 0) {
        ::close(fd);
        return -1;
    }
    return fd;
}

} // namespace

Test(core_integration, construction_demarre_un_listener_sur_le_port)
{
    Args args = makeArgs(48820);
    Core core(args);
    int client = connectClient(48820);

    cr_assert_geq(client, 0, "le client doit reussir a se connecter");
    ::close(client);
}

Test(core_integration, construction_peuple_la_carte_avec_des_ressources)
{
    Args args = makeArgs(48821);
    Core core(args);

    cr_assert_eq(args.getWidth(), 5);
    cr_assert_eq(args.getHeight(), 5);
}

Test(core_integration, construction_cree_les_equipes_demandees)
{
    Args args = makeArgs(48822);
    Core core(args);

    cr_assert_eq(args.getTeams().size(), 1);
    cr_assert_str_eq(args.getTeams()[0].c_str(), "red");
}

Test(core_integration, deux_serveurs_meme_port_lance_une_erreur)
{
    Args args = makeArgs(48823);
    Core first(args);
    bool threw = false;

    try {
        Core second(args);
    } catch (const std::runtime_error &) {
        threw = true;
    }
    cr_assert(threw);
}
