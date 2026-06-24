##
## EPITECH PROJECT, 2026
## zappy
## File description:
## player - etat du joueur (niveau, inventaire, mode, incantation)
##

import random

from strategy.Utils import (Mode, IncantationState, levels, RESOURCE_RARITY,
                    FOOD_CRITICAL, FOOD_HUNGRY, FOOD_OBJECTIVE)
from strategy.Inventory import Inventory
from pathfinding.Movement import Movement

class Player:
    def __init__(self, team_name, is_boss=True):
        self.team_name = team_name
        self.player_id = random.randint(1, 1000000)
        self.level = 1
        self.inventory = Inventory()
        self.mode = Mode.NORMAL
        self.movement = Movement(self.level)
        self.incantation_state = IncantationState.IDLE
        self.boss = is_boss
        self.can_subsist = False
        self.is_immobile = False
        self.immobility_reason = None

    def set_immobile(self, reason="incantation"):
        if self.boss:
            self.is_immobile = True
            self.immobility_reason = reason

    def set_mobile(self):
        if self.boss and self.is_immobile:
            self.is_immobile = False
            self.immobility_reason = None

    def can_team_upgrade(self, level):
        level_name = f"LEVEL_{level + 1}"
        if level_name not in levels.__members__:
            return False
        required_resources = getattr(levels, level_name).value
        if "total" not in self.inventory.team_inventory:
            return False
        team_inventory = self.inventory.team_inventory["total"]
        for resource, needed in required_resources.items():
            if resource == "Player":
                player_count = len([pid for pid in self.inventory.team_inventory
                                    if pid != "total"])
                if player_count < needed:
                    return False
                continue
            if team_inventory.get(resource.lower(), 0) < needed:
                return False
        return True

    def update_inventory(self, inventory_str):
        cleaned_str = inventory_str.strip().strip('[]').strip()
        items = [item.strip().strip("'\"") for item in cleaned_str.split(',')]
        if not items or items == [''] or items[0].split()[0] != "food":
            return
        for item in items:
            if not item:
                continue
            parts = item.strip().strip("']").split()
            if len(parts) == 2:
                try:
                    self.inventory.update(parts[0], int(parts[1].strip("']")))
                except ValueError:
                    pass

    def get_food_level(self):
        return self.inventory.get("food")

    def is_hungry(self):
        return self.get_food_level() < FOOD_HUNGRY and not self.can_subsist

    def is_very_hungry(self):
        if self.get_food_level() < FOOD_CRITICAL:
            self.can_subsist = False
            return True
        return False

    def food_objective(self):
        return FOOD_OBJECTIVE

    def can_upgrade(self):
        if self.is_hungry():
            return False
        return not self.inventory.get_missing_resource(self.level)

    def get_resource_priority_score(self, resource_name, quantity):
        if resource_name not in RESOURCE_RARITY:
            return 0
        missing = self.inventory.get_missing_resource(self.level)
        need_score = missing.get(resource_name, 0) * 100
        rarity_score = RESOURCE_RARITY[resource_name] * 5
        quantity_score = min(quantity, 5)
        return need_score + rarity_score + quantity_score
