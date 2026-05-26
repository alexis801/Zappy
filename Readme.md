# ZAPPY

> *"A tribute to Zaphod Beeblebrox"*

Zappy is a multiplayer network game where autonomous AI-controlled creatures evolve on a shared world called **Trantor**.
Players gather resources, survive, communicate, cooperate, and perform elevation rituals in order to reach the highest level before rival teams.

The project is composed of three independent programs:

* A game server handling the world simulation and networking
* A graphical client visualizing the world in real time
* An autonomous AI client controlling a player through the game protocol

---

## Overview

Zappy is a real-time multiplayer strategy and simulation project built around socket programming, asynchronous networking, and distributed AI behavior.

The world is represented as a toroidal map filled with food and minerals.
Each AI controls a drone capable of:

* Exploring the map
* Collecting resources
* Broadcasting messages
* Cooperating with allies
* Performing elevation rituals
* Spawning new players through reproduction

The first team with at least **6 players reaching level 8** wins the game.

---

## Architecture

```text
             +-------------------+
             |    zappy_gui      |
             |   Graphical UI    |
             +---------+---------+
                       |
                       |
               TCP Communication
                       |
                       v
+----------------------+----------------------+
|                 zappy_server               |
|--------------------------------------------|
|  World Simulation                          |
|  Resource Management                       |
|  Player Actions                            |
|  AI Command Queue                          |
|  Event Scheduler                           |
|  Poll-based Socket Multiplexing            |
+----------------------+----------------------+
                       ^
                       |
               TCP Communication
                       |
        +--------------+--------------+
        |                             |
+-------+--------+           +--------+-------+
|    zappy_ai    |           |    zappy_ai    |
| Autonomous AI  |    ...    | Autonomous AI  |
+----------------+           +----------------+
````

---

## Features

### Game World

* Toroidal map (wrap-around world)
* Dynamic resource spawning
* Real-time simulation
* Multi-team gameplay
* Elevation system
* Sound propagation system
* Vision cone mechanics

### Networking

* TCP socket communication
* Non-blocking architecture
* `poll()` multiplexing
* Buffered command queue
* Asynchronous event handling

### AI System

* Autonomous decision making
* Resource collection
* Team coordination
* Broadcast communication
* Ritual synchronization
* Reproduction management

### Graphical Interface

* Real-time world visualization
* Player tracking
* Resource rendering
* Event updates
* Network protocol monitoring

---

## Technologies

| Component | Language    |
| --------- | ----------- |
| Server    | C / C++     |
| GUI       | C++         |
| AI        | Free choice |

Recommended libraries:

* SFML (GUI)
* POSIX sockets
* poll()
* Makefile

---

# Game Mechanics

## Resources

The world contains several collectible resources:

| Resource  | Description          |
| --------- | -------------------- |
| food      | Survival resource    |
| linemate  | Elevation stone      |
| deraumere | Elevation stone      |
| sibur     | Elevation stone      |
| mendiane  | Elevation stone      |
| phiras    | Elevation stone      |
| thystame  | Rare elevation stone |

Resources are automatically regenerated over time according to density rules.

---

## Survival

Players consume food continuously.

* 1 food unit = 126 time units of survival
* Players die if food reaches 0

---

## Elevation System

Players evolve through collaborative rituals called **incantations**.

Each level requires:

* A minimum number of players
* Specific resources on the same tile

Example:

| Elevation | Players | Required Stones                  |
| --------- | ------- | -------------------------------- |
| 1 -> 2    | 1       | 1 linemate                       |
| 2 -> 3    | 2       | 1 linemate, 1 deraumere, 1 sibur |
| 3 -> 4    | 2       | 2 linemate, 1 sibur, 2 phiras    |

The final objective is reaching **level 8**.

---

## Vision System

Player vision increases with level.

A `Look` command returns visible tiles in a cone-shaped field of view.

Example:

```text
[player, food, linemate, player sibur]
```

---

## Broadcast System

Players can communicate using broadcasts:

```text
Broadcast Hello team
```

Other players receive:

```text
message K, Hello team
```

Where `K` represents the sound direction.

---

# Binaries

## Server

```bash
./zappy_server -p port -x width -y height -n team1 team2 -c clientsNb -f freq
```

### Arguments

| Flag | Description                 |
| ---- | --------------------------- |
| `-p` | Port number                 |
| `-x` | World width                 |
| `-y` | World height                |
| `-n` | Team names                  |
| `-c` | Authorized clients per team |
| `-f` | Time frequency              |

---

## GUI

```bash
./zappy_gui -p port -h machine
```

### Arguments

| Flag | Description     |
| ---- | --------------- |
| `-p` | Port number     |
| `-h` | Server hostname |

---

## AI Client

```bash
./zappy_ai -p port -n team -h machine
```

### Arguments

| Flag | Description     |
| ---- | --------------- |
| `-p` | Port number     |
| `-n` | Team name       |
| `-h` | Server hostname |

---

# Communication Protocol

## Connection Handshake

```text
SERVER -> WELCOME
CLIENT -> TEAM_NAME
SERVER -> CLIENT_NUM
SERVER -> X Y
```

---

## Main Commands

| Command       | Description          |
| ------------- | -------------------- |
| `Forward`     | Move forward         |
| `Right`       | Turn right           |
| `Left`        | Turn left            |
| `Look`        | Observe surroundings |
| `Inventory`   | Show inventory       |
| `Broadcast`   | Send message         |
| `Take`        | Pick up object       |
| `Set`         | Drop object          |
| `Fork`        | Spawn egg            |
| `Incantation` | Start elevation      |
| `Eject`       | Push players         |

---

# Building

## Requirements

* GCC / Clang
* Make
* CMake (optional)
* SFML (GUI)

---

## Build

```bash
make
```

Available rules:

```bash
make zappy_server
make zappy_gui
make zappy_ai
```

---

# Project Structure

```text
.
|-- source
|   |-- server/
|   |   |-- networking/
|   |   |-- world/
|   |   |-- commands/
|   |   `-- scheduler/
|   |-- gui/
|   |   |-- renderer/
|   |   |-- scenes/
|   |   `-- networking/
|   `-- ai/
|       |-- strategy/
|       |-- pathfinding/
|       `-- communication/
|-- include/
|-- assets/
|-- docs/
`-- Makefile
```

---

# Performance Requirements

The server must:

* Run in a single process
* Run in a single thread
* Use `poll()` for multiplexing
* Avoid active waiting
* Handle asynchronous clients
* Queue commands correctly

---

# Example Launch

## Start Server

```bash
./zappy_server -p 4242 -x 20 -y 20 -n Red Blue -c 4 -f 100
```

## Start GUI

```bash
./zappy_gui -p 4242 -h localhost
```

## Start AI

```bash
./zappy_ai -p 4242 -n Red -h localhost
```

---

# Development Goals

This project focuses on:

* Network programming
* Real-time simulation
* Event scheduling
* Artificial intelligence
* Multiplayer synchronization
* Software architecture
* Performance optimization

---

# License

This project is developed as part of the EPITECH curriculum.

Usage and redistribution depend on your school's policy and project guidelines.

---

# Authors

```
```
