/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** ResourceSpawner
*/

#include "ResourceSpawner.hpp"

namespace Zappy::Server {

ResourceSpawner::ResourceSpawner(unsigned seed) noexcept : _rng(seed)
{
}

} // namespace Zappy::Server
