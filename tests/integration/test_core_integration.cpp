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
namespace {

int connectAndJoin(int port, const std::string &team)
{
    int fd = connectClient(port);

    if (fd < 0)
        return -1;
    char buf[64];
    ::recv(fd, buf, sizeof(buf), 0);
    ::send(fd, (team + "\n").c_str(), team.size() + 1, 0);
    std::this_thread::sleep_for(std::chrono::milliseconds(80));
    ::recv(fd, buf, sizeof(buf), 0);
    return fd;
}

int connectAndGreetAsGui(int port)
{
    int fd = connectClient(port);

    if (fd < 0)
        return -1;
    char buf[2048];
    ::recv(fd, buf, sizeof(buf), 0);
    ::send(fd, "GRAPHIC\n", 8, 0);
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    ::recv(fd, buf, sizeof(buf), 0);
    return fd;
}

std::string sendAndReceive(int fd, const std::string &cmd)
{
    ::send(fd, cmd.c_str(), cmd.size(), 0);
    std::this_thread::sleep_for(std::chrono::milliseconds(150));
    char buf[256] = {};
    ssize_t n = ::recv(fd, buf, sizeof(buf) - 1, 0);
    if (n <= 0)
        return "";
    return std::string(buf, static_cast<std::size_t>(n));
}

} // namespace

Test(core_integration, inventaire_renvoie_la_nourriture)
{
    Args args = makeArgs(48828);
    Core core(args);
    std::thread server([&]() { core.run(); });

    waitForPortOpen(48828);
    int fd = connectAndJoin(48828, "red");
    cr_assert_geq(fd, 0);
    std::string response = sendAndReceive(fd, "Inventory\n");

    cr_assert(response.find("food") != std::string::npos,
        "reponse: '%s'", response.c_str());
    ::close(fd);
    server.detach();
    std::exit(0);
}

Test(core_integration, forward_renvoie_ok)
{
    Args args = makeArgs(48829);
    Core core(args);
    std::thread server([&]() { core.run(); });

    waitForPortOpen(48829);
    int fd = connectAndJoin(48829, "red");
    cr_assert_geq(fd, 0);
    std::string response = sendAndReceive(fd, "Forward\n");

    cr_assert(response.find("ok\n") != std::string::npos,
        "reponse: '%s'", response.c_str());
    ::close(fd);
    server.detach();
    std::exit(0);
}

Test(core_integration, look_renvoie_le_contenu_des_cases)
{
    Args args = makeArgs(48830);
    Core core(args);
    std::thread server([&]() { core.run(); });

    waitForPortOpen(48830);
    int fd = connectAndJoin(48830, "red");
    cr_assert_geq(fd, 0);
    std::string response = sendAndReceive(fd, "Look\n");

    cr_assert(response.find("[") != std::string::npos,
        "reponse: '%s'", response.c_str());
    ::close(fd);
    server.detach();
    std::exit(0);
}
Test(core_integration, commande_inconnue_renvoie_ko)
{
    Args args = makeArgs(48831);
    Core core(args);
    std::thread server([&]() { core.run(); });

    waitForPortOpen(48831);
    int fd = connectAndJoin(48831, "red");
    cr_assert_geq(fd, 0);
    std::string response = sendAndReceive(fd, "InvalidCmd\n");

    cr_assert_str_eq(response.c_str(), "ko\n");
    ::close(fd);
    server.detach();
    std::exit(0);
}
Test(core_integration, take_set_eject_renvoient_ok_ou_ko)
{
    Args args = makeArgs(48834);
    Core core(args);
    std::thread server([&]() { core.run(); });

    waitForPortOpen(48834);
    int fd = connectAndJoin(48834, "red");
    cr_assert_geq(fd, 0);
    std::string takeResponse = sendAndReceive(fd, "Take food\n");
    std::string setResponse = sendAndReceive(fd, "Set linemate\n");
    std::string ejectResponse = sendAndReceive(fd, "Eject\n");

    cr_assert(!takeResponse.empty());
    cr_assert(!setResponse.empty());
    cr_assert(!ejectResponse.empty());
    ::close(fd);
    server.detach();
    std::exit(0);
}

Test(core_integration, fork_renvoie_ok)
{
    Args args = makeArgs(48835);
    Core core(args);
    std::thread server([&]() { core.run(); });

    waitForPortOpen(48835);
    int fd = connectAndJoin(48835, "red");
    cr_assert_geq(fd, 0);
    std::string response = sendAndReceive(fd, "Fork\n");

    cr_assert(response.find("ok\n") != std::string::npos,
        "reponse: '%s'", response.c_str());
    ::close(fd);
    server.detach();
    std::exit(0);
}

Test(core_integration, connect_nbr_renvoie_un_chiffre)
{
    Args args = makeArgs(48836);
    Core core(args);
    std::thread server([&]() { core.run(); });

    waitForPortOpen(48836);
    int fd = connectAndJoin(48836, "red");
    cr_assert_geq(fd, 0);
    std::string response = sendAndReceive(fd, "Connect_nbr\n");

    cr_assert(!response.empty());
    ::close(fd);
    server.detach();
    std::exit(0);
}
Test(core_integration, gui_handshake_renvoie_la_taille_de_la_carte)
{
    Args args = makeArgs(48827);
    Core core(args);
    std::thread server([&]() { core.run(); });

    waitForPortOpen(48827);
    int fd = connectClient(48827);
    cr_assert_geq(fd, 0);
    readUntilNewline(fd);
    ::send(fd, "GRAPHIC\n", 8, 0);
    std::this_thread::sleep_for(std::chrono::milliseconds(150));
    char buf[2048] = {};
    ssize_t n = ::recv(fd, buf, sizeof(buf) - 1, 0);

    cr_assert_gt(n, 0);
    std::string response(buf, static_cast<std::size_t>(n));
    cr_assert(response.find("msz 5 5\n") != std::string::npos);
    ::close(fd);
    server.detach();
    std::exit(0);
}
Test(core_integration, gui_msz_renvoie_la_taille_de_la_carte)
{
    Args args = makeArgs(48832);
    Core core(args);
    std::thread server([&]() { core.run(); });

    waitForPortOpen(48832);
    int fd = connectClient(48832);
    cr_assert_geq(fd, 0);
    char buf[2048] = {};
    ::recv(fd, buf, sizeof(buf), 0);
    ::send(fd, "GRAPHIC\n", 8, 0);
    std::this_thread::sleep_for(std::chrono::milliseconds(150));
    ::recv(fd, buf, sizeof(buf), 0);
    std::string response = sendAndReceive(fd, "msz\n");

    cr_assert(response.find("msz 5 5\n") != std::string::npos,
        "reponse: '%s'", response.c_str());
    ::close(fd);
    server.detach();
    std::exit(0);
}

Test(core_integration, gui_sgt_renvoie_la_frequence)
{
    Args args = makeArgs(48833);
    Core core(args);
    std::thread server([&]() { core.run(); });

    waitForPortOpen(48833);
    int fd = connectClient(48833);
    cr_assert_geq(fd, 0);
    char buf[2048] = {};
    ::recv(fd, buf, sizeof(buf), 0);
    ::send(fd, "GRAPHIC\n", 8, 0);
    std::this_thread::sleep_for(std::chrono::milliseconds(150));
    ::recv(fd, buf, sizeof(buf), 0);
    std::string response = sendAndReceive(fd, "sgt\n");

    cr_assert(response.find("sgt ") != std::string::npos,
        "reponse: '%s'", response.c_str());
    ::close(fd);
    server.detach();
    std::exit(0);
}
Test(core_integration, gui_repond_aux_requetes_bct_ppo_plv_pin)
{
    Args args = makeArgs(48837);
    Core core(args);
    std::thread server([&]() { core.run(); });

    waitForPortOpen(48837);
    int player = connectAndJoin(48837, "red");
    cr_assert_geq(player, 0);
    int gui = connectAndGreetAsGui(48837);
    cr_assert_geq(gui, 0);
    std::string mct = sendAndReceive(gui, "mct\n");

    cr_assert(mct.find("bct") != std::string::npos,
        "mct: '%s'", mct.c_str());
    ::close(player);
    ::close(gui);
    server.detach();
    std::exit(0);
}

Test(core_integration, gui_commande_inconnue_renvoie_suc)
{
    Args args = makeArgs(48838);
    Core core(args);
    std::thread server([&]() { core.run(); });

    waitForPortOpen(48838);
    int fd = connectClient(48838);
    cr_assert_geq(fd, 0);
    char buf[2048] = {};
    ::recv(fd, buf, sizeof(buf), 0);
    ::send(fd, "GRAPHIC\n", 8, 0);
    std::this_thread::sleep_for(std::chrono::milliseconds(150));
    ::recv(fd, buf, sizeof(buf), 0);
    std::string response = sendAndReceive(fd, "unknown\n");

    cr_assert(response.find("suc\n") != std::string::npos,
        "reponse: '%s'", response.c_str());
    ::close(fd);
    server.detach();
    std::exit(0);
}

Test(core_integration, gui_sst_modifie_la_frequence)
{
    Args args = makeArgs(48839);
    Core core(args);
    std::thread server([&]() { core.run(); });

    waitForPortOpen(48839);
    int fd = connectClient(48839);
    cr_assert_geq(fd, 0);
    char buf[2048] = {};
    ::recv(fd, buf, sizeof(buf), 0);
    ::send(fd, "GRAPHIC\n", 8, 0);
    std::this_thread::sleep_for(std::chrono::milliseconds(150));
    ::recv(fd, buf, sizeof(buf), 0);
    std::string response = sendAndReceive(fd, "sst 200\n");

    cr_assert(response.find("sst") != std::string::npos,
        "reponse: '%s'", response.c_str());
    ::close(fd);
    server.detach();
    std::exit(0);
}
