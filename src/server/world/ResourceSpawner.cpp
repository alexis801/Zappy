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

void ResourceSpawner::initialSpawn(Map &map) noexcept
{
    int width = map.getWidth();
    int height = map.getHeight();
    std::uniform_int_distribution<int> xDist(0, width - 1);
    std::uniform_int_distribution<int> yDist(0, height - 1);

    for (std::size_t i = 0; i < RESOURCE_COUNT; ++i) {
        Resource resource = static_cast<Resource>(i);
        int target = getTargetQuantity(resource, width, height);

        for (int placed = 0; placed < target; ++placed)
            map.at(xDist(_rng), yDist(_rng)).add(resource, 1);
    }
}

void ResourceSpawner::respawn(Map &map) noexcept
{
    int width = map.getWidth();
    int height = map.getHeight();
    std::uniform_int_distribution<int> xDist(0, width - 1);
    std::uniform_int_distribution<int> yDist(0, height - 1);

    for (std::size_t i = 0; i < RESOURCE_COUNT; ++i) {
        Resource resource = static_cast<Resource>(i);
        int target = getTargetQuantity(resource, width, height);
        int missing = target - countOnMap(resource, map);

        for (int placed = 0; placed < missing; ++placed)
            map.at(xDist(_rng), yDist(_rng)).add(resource, 1);
    }
}

} // namespace Zappy::Server
