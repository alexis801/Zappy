/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Client
*/

#include "Client.hpp"
#include "Macros.hpp"
#include <algorithm>
#include <array>
#include <sys/socket.h>
#include <utility>

namespace Zappy::Server {

namespace {
constexpr std::size_t READ_CHUNK = 4096;
}

Client::Client(Socket socket) noexcept : _socket(std::move(socket))
{
}

bool Client::receive()
{
    std::array<char, READ_CHUNK> buffer;
    ssize_t count = ::recv(_socket.getFd(), buffer.data(), buffer.size(), 0);

    if (count <= 0)
        return false;
    feed(std::string_view(buffer.data(), static_cast<std::size_t>(count)));
    return true;
}

void Client::feed(std::string_view data)
{
    _readBuffer.feed(data);
}

std::optional<std::string> Client::popLine()
{
    return _readBuffer.popLine();
}

bool Client::send(std::string_view data)
{
    return _socket.write(data);
}

void Client::setAsPlayer(std::string teamName) noexcept
{
    _state = State::Player;
    _teamName = std::move(teamName);
}

void Client::setAsGui() noexcept
{
    _state = State::Gui;
}

void Client::spawnPlayer(int width, int height, int id)
{
    _player = Player::spawnRandom(width, height, id);
}

void Client::spawnFromEgg(const Egg &egg, int id)
{
    _player = Player::spawnFromEgg(egg, id);
}

Player *Client::getPlayer() noexcept
{
    if (!_player.has_value())
        return nullptr;
    return &*_player;
}

bool Client::enqueueCommand(
    std::string text, long nowMillis, long durationMillis)
{
    if (_commands.size() >= MAX_PENDING_REQUESTS)
        return false;
    long startMillis = _commands.empty()
        ? nowMillis
        : std::max(nowMillis, _commands.back().deadlineMillis);

    _commands.push({std::move(text), startMillis + durationMillis});
    return true;
}

long Client::getNextDeadlineMillis() const noexcept
{
    return _commands.front().deadlineMillis;
}

Command Client::popCommand() noexcept
{
    Command cmd = std::move(_commands.front());

    _commands.pop();
    return cmd;
}

} // namespace Zappy::Server
