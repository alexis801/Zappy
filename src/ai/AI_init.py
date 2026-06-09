##
## EPITECH PROJECT, 2026
## zappyGotten
## File description:
## AI_init
##

from dataclasses import dataclass
from enum import Enum

@dataclass
class Inventory:
    food: int = 0
    linemate: int = 0
    deraumere: int = 0
    sibur: int = 0
    mendiane: int = 0
    phiras: int = 0
    thystame: int = 0

class Tile:
    def __init__(self):
        self.food = 0
        self.linemate = 0
        self.deraumere = 0
        self.sibur = 0
        self.mendiane = 0
        self.phiras = 0
        self.thystame = 0
        self.players = 0

class State(Enum):
    SURVIVE = 1
    COLLECT = 2
    EXPLORE = 3
