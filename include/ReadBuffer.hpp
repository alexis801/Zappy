/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** ReadBuffer
*/

#ifndef ZAPPY_READBUFFER_HPP_
#define ZAPPY_READBUFFER_HPP_

#include <optional>
#include <string>
#include <string_view>

namespace Zappy::Server {

/**
 * @class ReadBuffer
 * @brief Accumule des data recues et les decoupe en lignes \\n par \\n
 */
class ReadBuffer {
public:
    /**
     * @brief Ajoute des data au buffer
     */
    void feed(std::string_view data);

    /**
     * @brief Retire la prochaine ligne complete, si elle existe
     */
    [[nodiscard]] std::optional<std::string> popLine();

    /**
     * @brief Vrai si le buffer ne contient aucun byte
     */
    [[nodiscard]] bool isEmpty() const noexcept { return _bytes.empty(); }

private:
    std::string _bytes;
};

} // namespace Zappy::Server

#endif
