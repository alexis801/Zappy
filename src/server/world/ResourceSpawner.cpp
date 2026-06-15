/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** ResourceSpawner
*/

#include "ResourceSpawner.hpp"
#include <algorithm>
#include <cmath>

namespace Zappy::Server {

ResourceSpawner::ResourceSpawner(unsigned seed) noexcept : _rng(seed)
{
}

int ResourceSpawner::getTargetQuantity(
    Resource resource, int width, int height) noexcept
{
    double density = RESOURCE_DENSITY[static_cast<std::size_t>(resource)];
    int quantity = static_cast<int>(std::floor(width * height * density));

    return std::max(quantity, 1);
}

int ResourceSpawner::countOnMap(
    Resource resource, const Map &map) noexcept
{
    int total = 0;

    for (int y = 0; y < map.getHeight(); ++y) {
        for (int x = 0; x < map.getWidth(); ++x)
            total += map.at(x, y).get(resource);
    }
    return total;
}

} // namespace Zappy::Server
