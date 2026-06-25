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
namespace {

std::string readUntilNewline(int fd, int maxBytes = 4096)
{
    std::string out;
    char buf[1024];

    while (static_cast<int>(out.size()) < maxBytes) {
        ssize_t n = ::recv(fd, buf, sizeof(buf), 0);
        if (n <= 0)
            break;
        out.append(buf, static_cast<std::size_t>(n));
        if (out.find('\n') != std::string::npos)
            break;
    }
    return out;
}

void waitForPortOpen(int port)
{
    for (int i = 0; i < 20; ++i) {
        int fd = connectClient(port);
        if (fd >= 0) {
            ::close(fd);
            return;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
}

} // namespace

Test(core_integration, client_recoit_welcome_a_la_connexion)
{
    Args args = makeArgs(48824);
    Core core(args);
    std::thread server([&]() { core.run(); });

    waitForPortOpen(48824);
    int fd = connectClient(48824);
    cr_assert_geq(fd, 0);
    std::string welcome = readUntilNewline(fd);

    cr_assert_str_eq(welcome.c_str(), "WELCOME\n");
    ::close(fd);
    server.detach();
    std::exit(0);
}

Test(core_integration, handshake_player_renvoie_slots_et_carte)
{
    Args args = makeArgs(48825);
    Core core(args);
    std::thread server([&]() { core.run(); });

    waitForPortOpen(48825);
    int fd = connectClient(48825);
    cr_assert_geq(fd, 0);
    readUntilNewline(fd);
    ::send(fd, "red\n", 4, 0);
    char buf[64] = {};
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    ssize_t n = ::recv(fd, buf, sizeof(buf) - 1, 0);

    cr_assert_gt(n, 0);
    std::string response(buf, static_cast<std::size_t>(n));
    cr_assert(response.find("5 5\n") != std::string::npos,
        "reponse: '%s'", response.c_str());
    ::close(fd);
    server.detach();
    std::exit(0);
}

Test(core_integration, equipe_inconnue_ferme_la_connexion)
{
    Args args = makeArgs(48826);
    Core core(args);
    std::thread server([&]() { core.run(); });

    waitForPortOpen(48826);
    int fd = connectClient(48826);
    cr_assert_geq(fd, 0);
    readUntilNewline(fd);
    ::send(fd, "unknown\n", 8, 0);
    char buf[64];
    ssize_t n = ::recv(fd, buf, sizeof(buf), 0);

    cr_assert_eq(n, 0, "le serveur ferme la connexion");
    ::close(fd);
    server.detach();
    std::exit(0);
}
