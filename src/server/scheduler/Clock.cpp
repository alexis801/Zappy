/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Clock
*/

#include "Clock.hpp"
#include <chrono>

namespace Zappy::Server {

namespace {

long currentMillis() noexcept
{
    using namespace std::chrono;
    return duration_cast<milliseconds>(
        steady_clock::now().time_since_epoch()).count();
}

} // namespace

Clock::Clock(int frequency) noexcept
    : _frequency(frequency), _startMillis(currentMillis())
{
}

void Clock::setFrequency(int frequency) noexcept
{
    _frequency = frequency;
}

long Clock::getElapsedMillis() const noexcept
{
    return currentMillis() - _startMillis;
}

long Clock::getDurationMillis(int actionCost) const noexcept
{
    return (static_cast<long>(actionCost) * 1000) / _frequency;
}

long Clock::getRemainingMillis(long deadlineMillis) const noexcept
{
    long remaining = deadlineMillis - getElapsedMillis();

    return remaining > 0 ? remaining : 0;
}

} // namespace Zappy::Server
