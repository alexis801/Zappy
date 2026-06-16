/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** ResourceSpawner
*/

#ifndef ZAPPY_RESOURCE_SPAWNER_HPP_
#define ZAPPY_RESOURCE_SPAWNER_HPP_

#include "Macros.hpp"
#include "Map.hpp"
#include <random>

namespace Zappy::Server {

using Rng = std::mt19937;

class ResourceSpawner {
public:
    explicit ResourceSpawner(unsigned seed) noexcept;

    [[nodiscard]] static int getTargetQuantity(
        Resource resource, int width, int height) noexcept;

    [[nodiscard]] static int countOnMap(
        Resource resource, const Map &map) noexcept;

    void initialSpawn(Map &map) noexcept;

private:
    Rng _rng;
};

} // namespace Zappy::Server

#endif
