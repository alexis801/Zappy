/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** test_clock
*/

#include <criterion/criterion.h>
#include "Clock.hpp"
#include <chrono>
#include <thread>

using Zappy::Server::Clock;

Test(clock, cost_to_millis_par_defaut_a_100)
{
    Clock clock(100);

    cr_assert_eq(clock.getDurationMillis(7), 70);
    cr_assert_eq(clock.getDurationMillis(42), 420);
    cr_assert_eq(clock.getDurationMillis(300), 3000);
}

Test(clock, cost_to_millis_a_frequence_lente)
{
    Clock clock(1);

    cr_assert_eq(clock.getDurationMillis(7), 7000);
}

Test(clock, cost_to_millis_a_frequence_rapide)
{
    Clock clock(1000);

    cr_assert_eq(clock.getDurationMillis(300), 300);
}

Test(clock, set_frequency_change_la_duree_des_actions)
{
    Clock clock(100);

    cr_assert_eq(clock.getDurationMillis(7), 70);
    clock.setFrequency(50);
    cr_assert_eq(clock.getFrequency(), 50);
    cr_assert_eq(clock.getDurationMillis(7), 140);
}

Test(clock, now_millis_avance_avec_le_temps)
{
    Clock clock(100);
    long start = clock.getElapsedMillis();

    std::this_thread::sleep_for(std::chrono::milliseconds(60));
    long after = clock.getElapsedMillis();

    cr_assert(after - start >= 50);
}

Test(clock, millis_until_donne_le_temps_restant)
{
    Clock clock(100);
    long deadline = clock.getElapsedMillis() + 100;

    cr_assert(clock.getRemainingMillis(deadline) >= 80);
    cr_assert(clock.getRemainingMillis(deadline) <= 100);
}

Test(clock, millis_until_renvoie_zero_si_deja_passe)
{
    Clock clock(100);
    long past = clock.getElapsedMillis() - 50;

    cr_assert_eq(clock.getRemainingMillis(past), 0);
}
