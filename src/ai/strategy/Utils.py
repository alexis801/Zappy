##
## EPITECH PROJECT, 2026
## zappy
## File description:
## config - enums, constantes et helpers partages de la strategie
##

import base64
import binascii
import hashlib

from enum import Enum

# Secret partage par tous les binaires de l'equipe (compile dans l'AI).
# Une equipe adverse ne le possede pas : elle ne peut ni lire ni forger
# nos broadcasts, meme en connaissant notre nom d'equipe.
BROADCAST_SECRET = "Z4ph0d-B33bl3br0x"
_KEYSTREAM_LEN = 256


def broadcast_key(team_name):
    # Keystream derive par hash de (secret + nom d'equipe) : chaque octet
    # depend a la fois du secret et du nom, donc deux equipes au meme binaire
    # mais nom different ne peuvent pas se lire, et le nom ne fuit pas en clair.
    seed = (BROADCAST_SECRET + "|" + team_name).encode()
    stream = b""
    counter = 0
    while len(stream) < _KEYSTREAM_LEN:
        stream += hashlib.sha256(seed + bytes([counter])).digest()
        counter += 1
    return stream


def encrypt_message(plain, key):
    data = plain.encode()
    cipher = bytes(b ^ key[i % len(key)] for i, b in enumerate(data))
    return base64.b64encode(cipher).decode()


def decrypt_message(payload, key):
    try:
        cipher = base64.b64decode(payload, validate=True)
    except (binascii.Error, ValueError):
        return None
    plain = bytes(b ^ key[i % len(key)] for i, b in enumerate(cipher))
    try:
        return plain.decode()
    except UnicodeDecodeError:
        return None


class IncantationState(Enum):
    IDLE = 0
    GATHERING_PLAYERS = 1
    MASTER_DROPPING = 2
    EXECUTING = 3
    RECOVERY = 4

class Mode(Enum):
    NORMAL = "normal"
    SURVIVAL = "survival"

resource_map = {"food": "f", "linemate": "l", "deraumere": "d", "sibur": "s",
                "mendiane": "m", "phiras": "p", "thystame": "t"}

class levels(Enum):
    LEVEL_2 = {"Player": 1, "Linemate": 1}
    LEVEL_3 = {"Player": 2, "Linemate": 1, "Deraumere": 1, "Sibur": 1}
    LEVEL_4 = {"Player": 2, "Linemate": 2, "Sibur": 1, "Phiras": 2}
    LEVEL_5 = {"Player": 4, "Linemate": 1, "Deraumere": 1, "Sibur": 2, "Phiras": 1}
    LEVEL_6 = {"Player": 4, "Linemate": 1, "Deraumere": 2, "Sibur": 1, "Mendiane": 3}
    LEVEL_7 = {"Player": 6, "Linemate": 1, "Deraumere": 2, "Sibur": 3, "Phiras": 1}
    LEVEL_8 = {"Player": 6, "Linemate": 2, "Deraumere": 2, "Sibur": 2,
               "Mendiane": 2, "Phiras": 2, "Thystame": 1}

COHORT_SIZE = 6
TARGET_PLAYERS = 6
TEAMMATE_TTL = 15.0
REPOP_PERIOD = 50
FOOD_CRITICAL = 20
FOOD_HUNGRY = 40
FOOD_SURVIVAL_TARGET = 100
FOOD_RECOVERY_TARGET = 60
FOOD_OBJECTIVE = 90

RESOURCE_RARITY = {
    "food": 1, "linemate": 2, "deraumere": 3, "sibur": 4,
    "mendiane": 5, "phiras": 6, "thystame": 7,
}

def required_players_for(level):
    name = f"LEVEL_{level + 1}"
    if name not in levels.__members__:
        return 0
    return getattr(levels, name).value.get("Player", 0)

def players_needed(level):
    return required_players_for(level)
