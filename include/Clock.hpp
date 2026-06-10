/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Clock
*/

#ifndef ZAPPY_CLOCK_HPP_
#define ZAPPY_CLOCK_HPP_

namespace Zappy::Server {

/**
 * @class Clock
 * @brief Mesure le temps et calcule la duree des actions selon la vitesse f
 */
class Clock {
public:
    explicit Clock(int frequency) noexcept;

    [[nodiscard]] int getFrequency() const noexcept { return _frequency; }

    /**
     * @brief Change la vitesse du jeu (commande sst de la GUI)
     */
    void setFrequency(int frequency) noexcept;

    /**
     * @brief Temps actuel en ms depuis le demarrage de l'horloge
     */
    [[nodiscard]] long getElapsedMillis() const noexcept;

    /**
     * @brief Duree d'une action en ms : cout * 1000 / f
     */
    [[nodiscard]] long getDurationMillis(int actionCost) const noexcept;

    /**
     * @brief Ms restant jusqu'au temps donne, 0 si deja passe
     */
    [[nodiscard]] long getRemainingMillis(long deadlineMillis) const noexcept;

private:
    int _frequency;
    long _startMillis;
};

} // namespace Zappy::Server

#endif
