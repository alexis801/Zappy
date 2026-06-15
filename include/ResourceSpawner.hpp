/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** ResourceSpawner
*/

#ifndef ZAPPY_RESOURCE_SPAWNER_HPP_
#define ZAPPY_RESOURCE_SPAWNER_HPP_

#include <random>

namespace Zappy::Server {

using Rng = std::mt19937;

class ResourceSpawner {
public:
    explicit ResourceSpawner(unsigned seed) noexcept;

private:
    Rng _rng;
};

} // namespace Zappy::Server

#endif
