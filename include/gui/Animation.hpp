/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Animation.hpp (GUI)
*/

#ifndef ZAPPY_GUI_ANIMATION_HPP_
#define ZAPPY_GUI_ANIMATION_HPP_

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

namespace Zappy::Gui {

class Animation {
public:
    bool load(const std::string &prefix, std::size_t count);
    [[nodiscard]] std::size_t size() const noexcept { return _textures.size(); }
    [[nodiscard]] const sf::Texture &frameAt(std::size_t i) const
    {
        return _textures[i];
    }

private:
    std::vector<sf::Texture> _textures;
};

} // namespace Zappy::Gui

#endif // ZAPPY_GUI_ANIMATION_HPP_