/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** ResourceSpawner
*/

#include "ResourceSpawner.hpp"
#include <cmath>

namespace Zappy::Server {

ResourceSpawner::ResourceSpawner(unsigned seed) noexcept : _rng(seed)
{
}

int ResourceSpawner::getTargetQuantity(
    Resource resource, int width, int height) noexcept
{
    double density = RESOURCE_DENSITY[static_cast<std::size_t>(resource)];

    return static_cast<int>(std::floor(width * height * density));
}

} // namespace Zappy::Server
