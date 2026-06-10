#!/usr/bin/env python3

##
## EPITECH PROJECT, 2026
## zappyGotten
## File description:
## AI_init
##

from dataclasses import dataclass
from enum import Enum
import sys, getopt

@dataclass
class Inventory:
    food: int = 0
    linemate: int = 0
    deraumere: int = 0
    sibur: int = 0
    mendiane: int = 0
    phiras: int = 0
    thystame: int = 0
    
    def total_resources(self) -> int:
        return (self.linemate + self.deraumere + self.sibur + 
                self.mendiane + self.phiras + self.thystame)
    
    def status(self):
        return (f"Food: {self.food}, Linemate: {self.linemate}, "
                f"Deraumere: {self.deraumere}, Sibur: {self.sibur}, "
                f"Mendiane: {self.mendiane}, Phiras: {self.phiras}, "
                f"Thystame: {self.thystame}")

class Direction(Enum):
    UP = 0
    LEFT = 1
    DOWN = 2
    RIGHT = 3


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

class Resource(Enum):
    FOOD = "food"
    LINEMATE = "linemate"
    DERAUMERE = "deraumere"
    SIBUR = "sibur"
    MENDIANE = "mendiane"
    PHIRAS = "phiras"
    THYSTAME = "thystame"

class SurvivalAI:
    FOOD_THRESHOLD = 10

    def choose_state(self, inventory):
        if inventory.food < self.FOOD_THRESHOLD:
            return State.SURVIVE
        return State.COLLECT

def parser(argv):
    port = None
    team = None
    host = "localhost"

    try:
        opts, args = getopt.getopt(argv, "p:n:h:", ["help"])
    except getopt.GetoptError:
        print("USAGE: ./zappy_ai -p port -n name -h machine")
        sys.exit(2)

    for opt, arg in opts:
        if opt == "--help":
            print("USAGE: ./zappy_ai -p port -n name -h machine")
            sys.exit(0)

        elif opt == "-p":
            port = int(arg)

        elif opt == "-n":
            team = arg

        elif opt == "-h":
            host = arg

    return port, team, host

def main(argv):
    port, team, host = parser(argv)

    print(f"Port: {port}")
    print(f"Team: {team}")
    print(f"Host: {host}")

if __name__ == "__main__":
    main(sys.argv[1:])