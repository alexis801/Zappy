/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Elevation
*/

#ifndef ZAPPY_ELEVATION_HPP_
#define ZAPPY_ELEVATION_HPP_

#include <array>

namespace Zappy {

inline constexpr int MAX_LEVEL = 8;

// Rituel d'elevation : conditions pour passer du niveau N a N+1 sur une meme
// case (joueurs de meme niveau + pierres). Indexe par (niveau de depart - 1).
struct ElevationRequirement {
    int players;
    int linemate;
    int deraumere;
    int sibur;
    int mendiane;
    int phiras;
    int thystame;
};

// ELEVATION[0] = passage 1->2, etc.
inline constexpr std::array<ElevationRequirement, MAX_LEVEL - 1> ELEVATION = {{
    //         players lin der sib men phi thy
    /* 1->2 */ {1, 1, 0, 0, 0, 0, 0},
    /* 2->3 */ {2, 1, 1, 1, 0, 0, 0},
    /* 3->4 */ {2, 2, 0, 1, 0, 2, 0},
    /* 4->5 */ {4, 1, 1, 2, 0, 1, 0},
    /* 5->6 */ {4, 1, 2, 1, 3, 0, 0},
    /* 6->7 */ {6, 1, 2, 3, 0, 1, 0},
    /* 7->8 */ {6, 2, 2, 2, 2, 2, 1},
}};

} // namespace Zappy

#endif
