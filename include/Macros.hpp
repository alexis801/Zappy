/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Macros
*/

#ifndef ZAPPY_MACROS_HPP_
#define ZAPPY_MACROS_HPP_

#include <array>
#include <cstddef>
#include <string_view>

namespace Zappy {

// Frequence / temps
// f = inverse du temps d'execution. Une action dure (cout / f) secondes.
inline constexpr int DEFAULT_FREQUENCY = 100;

// Valeurs par defaut du serveur (utilisees si l'option n'est pas passee)
inline constexpr int DEFAULT_PORT = 4242;
inline constexpr int DEFAULT_WIDTH = 10;
inline constexpr int DEFAULT_HEIGHT = 10;
inline constexpr int DEFAULT_CLIENTS_PER_TEAM = 4;
inline constexpr std::string_view DEFAULT_TEAM_NAME = "default";

// Vie des joueurs
// unites de nourriture au depart
inline constexpr int LIFE_UNITS_AT_START = 10;
// 1 nourriture = 126 unites de temps (vie de base = 10 * 126 = 1260)
inline constexpr int TIME_UNITS_PER_FOOD = 126;

// Vision
inline constexpr int VISION_AT_LEVEL_1 = 1;      // 1 case devant au niveau 1

// Reseau
inline constexpr int MAX_PENDING_REQUESTS = 10;  // 10 requetes max sans reponse

// Ressources
// respawn toutes les 20 unites de temps
inline constexpr int RESOURCE_SPAWN_INTERVAL = 20;

// Ordre = table de densites et indexation du protocole GUI
enum class Resource : std::size_t {
    FOOD = 0,
    LINEMATE,
    DERAUMERE,
    SIBUR,
    MENDIANE,
    PHIRAS,
    THYSTAME,
};
inline constexpr std::size_t RESOURCE_COUNT = 7;

// Densites de spawn : quantite = larg * h * d
inline constexpr std::array<double, RESOURCE_COUNT> RESOURCE_DENSITY = {{
    0.5,   // FOOD
    0.3,   // LINEMATE
    0.15,  // DERAUMERE
    0.1,   // SIBUR
    0.1,   // MENDIANE
    0.08,  // PHIRAS
    0.05,  // THYSTAME
}};

// Noms du protocole joueur (commande Inventory)
inline constexpr std::array<std::string_view, RESOURCE_COUNT>
    RESOURCE_NAMES = {{
    "food",
    "linemate",
    "deraumere",
    "sibur",
    "mendiane",
    "phiras",
    "thystame",
}};

// Couts des actions, en unites de temps
namespace action_cost {
inline constexpr int FORWARD     = 7;
inline constexpr int RIGHT       = 7;
inline constexpr int LEFT        = 7;
inline constexpr int LOOK        = 7;
inline constexpr int INVENTORY   = 1;
inline constexpr int BROADCAST   = 7;
inline constexpr int FORK        = 42;
inline constexpr int EJECT       = 7;
inline constexpr int TAKE        = 7;
inline constexpr int SET         = 7;
inline constexpr int INCANTATION = 300;
inline constexpr int CONNECT_NBR = 0;
} // namespace action_cost

// Auth GUI
inline constexpr std::string_view GRAPHIC_TEAM_NAME = "GRAPHIC";

// Incantation : niveau max et truc equis pour passer du niveau N a N+1
inline constexpr int MAX_PLAYER_LEVEL = 8;

// Victoire : 6 joueurs au niveau max
inline constexpr int VICTORY_REQUIRED_PLAYERS = 6;

struct IncantationRequirement {
    int players;
    std::array<int, RESOURCE_COUNT - 1> stones;
};

// Indexe par niveau de depart
inline constexpr std::array<IncantationRequirement, 7> INCANTATION_TABLE = {{
    {1, {1, 0, 0, 0, 0, 0}},
    {2, {1, 1, 1, 0, 0, 0}},
    {2, {2, 0, 1, 0, 2, 0}},
    {4, {1, 1, 2, 0, 1, 0}},
    {4, {1, 2, 1, 3, 0, 0}},
    {6, {1, 2, 3, 0, 1, 0}},
    {6, {2, 2, 2, 2, 2, 1}},
}};

} // namespace Zappy

#endif
