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

BROADCAST_SECRET = "N4t4nk-Zappy-2026-x9K2mQ7vR3pL-Tr4nt0r"
_KEYSTREAM_LEN = 256


def broadcast_key(team_name):
    secret_seed = (BROADCAST_SECRET + "|" + team_name).encode()
    secret_key_bytes = b""
    block_index = 0
    while len(secret_key_bytes) < _KEYSTREAM_LEN:
        secret_key_bytes += hashlib.sha256(
            secret_seed + bytes([block_index])).digest()
        block_index += 1
    return secret_key_bytes


def encrypt_message(readable_message, secret_key_bytes):
    readable_bytes = readable_message.encode()
    encrypted_bytes = bytes(
        message_byte ^ secret_key_bytes[position % len(secret_key_bytes)]
        for position, message_byte in enumerate(readable_bytes))
    return base64.b64encode(encrypted_bytes).decode()


def decrypt_message(encrypted_message, secret_key_bytes):
    try:
        encrypted_bytes = base64.b64decode(encrypted_message, validate=True)
    except (binascii.Error, ValueError):
        return None
    decrypted_bytes = bytes(
        encrypted_byte ^ secret_key_bytes[position % len(secret_key_bytes)]
        for position, encrypted_byte in enumerate(encrypted_bytes))
    try:
        return decrypted_bytes.decode()
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
