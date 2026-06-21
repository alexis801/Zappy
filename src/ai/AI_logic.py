#!/usr/bin/env python3

##
## EPITECH PROJECT, 2026
## zappyGotten
## File description:
## AI_init
##

from dataclasses import dataclass
from enum import Enum


from Inventory import *
from communication.AiClient import *

class Direction(Enum):
    FORWARD = 0
    LEFT = 1
    BACKWARD = 2
    RIGHT = 3

class State(Enum):
    SURVIVE = 1
    COLLECT = 2
    EXPLORE = 3

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

class Resource(Enum):
    FOOD = "food"
    LINEMATE = "linemate"
    DERAUMERE = "deraumere"
    SIBUR = "sibur"
    MENDIANE = "mendiane"
    PHIRAS = "phiras"
    THYSTAME = "thystame"

class SurvivalAI:
    FOOD_CRITICAL = 5
    FOOD_LOW = 10
    FOOD_COMFORTABLE = 25

    def __init__(self, client: ZappyClient):
        self.client = client
        self.inventory = Inventory()
        self.state = State.SURVIVE
        self.current_direction = Direction.FORWARD
        self.position_x = 0
        self.position_y = 0
        self.level = 1

    def update_state(self) -> State:
        if self.inventory.food <= self.FOOD_CRITICAL:
            return State.SURVIVE
        elif self.inventory.food <= self.FOOD_LOW:
            return State.COLLECT
        else:
            return State.EXPLORE
        
    def act(self) -> None:
        self.state = self.update_state()

        #regarder autour pour mettre a jour l'objectif
        self.client.send_command("Look")
        look_response = self.client.receive_response()
        if not look_response:
            print("cannot look")
            return
    
        #mettre a jour l'inventaire
        self.inventory()

        #quel action
        if self.state == State.SURVIVE:
            self.survive()
        elif self.state == State.COLLECT:
            self.collect()
        else:
            self.explore()

    def survive(self) -> None:
        None
    
    def collect(self) -> None:
        None

    def explore(self) -> None:
        None

    def go_forward(self) -> None:
        self.client.send_command("Forward")
        response = self.client.receive_response()
        if response == 'ok':
            self._update_position()

    def go_right(self) -> None:
        self.client.send_command("Right")
        response = self.client.receive_response()
        if response == 'ok':
            self.current_direction = Direction((self.current_direction.value + 1) % 4)

    def go_left(self) -> None:
        self.client.send_command("Left")
        response = self.client.receive_response()
        if response == 'ok':
            self.current_direction = Direction((self.current_direction.value - 1) % 4)

    def _update_position(self) -> None:
        if self.current_direction == Direction.FORWARD:
            self.position_y -= 1
        elif self.current_direction == Direction.RIGHT:
            self.position_x += 1
        elif self.current_direction == Direction.BACKWARD:
            self.position_y += 1
        elif self.current_direction == Direction.LEFT:
            self.position_x -= 1

    def store_inventory(self, inventory: str) -> None:
        inventory = inventory.strip("[]")
        items = inventory.split(",")
        
        for item in items:
            parts = item.strip().split()
            if len(parts) >= 2:
                resource_name = parts[0]
                try:
                    quantity = int(parts[1])
                    setattr(self.inventory, resource_name, quantity)
                except ValueError:
                    pass

    def inventory(self) -> None:
        self.client.send_command("Inventory")
        response = self.client.receive_response()   
        if response:
            self.store_inventory(response)
    
    def broadcast_text(self) -> None:
        self.client.send_command("Broadcast")
        response = self.client.receive_response()
        if response == 'ok':
            self.client.send_command(input("Enter message: "))

    def fork_player(self) -> None:
        self.client.send_command("Fork")
        response = self.client.receive_response()

    def start_incantation(self) -> None:
        self.client.send_command("Incantation")
        response = self.client.receive_response()

    def unused_team(self) -> None:
        self.client.send_command("Connect_nbr ")
        response = self.client.receive_response()
        print(response)

    def take_object(self, object) -> None:
        self.client.send_command("Take " + object)
        response = self.client.receive_response()
        if response == 'ok':
            print("take " + response)

    def set_object_down(self, object) -> None:
        self.client.send_command("Set " + object)
        response = self.client.receive_response()
        if response == 'ok':
            print("set " + response)
