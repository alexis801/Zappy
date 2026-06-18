#!/usr/bin/env python3

##
## EPITECH PROJECT, 2026
## zappy
## File description:
## inventory - inventaire individuel et agregation d'equipe
##

from collections import Counter

from config import levels, RESOURCE_RARITY


class Inventory:
    def __init__(self):
        self.resources = {
            "food": 0, "linemate": 0, "deraumere": 0, "sibur": 0,
            "mendiane": 0, "phiras": 0, "thystame": 0,
        }
        self.team_inventory = {}

    def recompute_total(self):
        total = Counter()
        for player_id, inventory in self.team_inventory.items():
            if player_id == 'total':
                continue
            total.update(inventory)
        self.team_inventory['total'] = dict(total)

    def update_team_inventory(self, player_id):
        if player_id not in self.team_inventory:
            self.team_inventory[player_id] = self.resources.copy()
        self.recompute_total()

    def register_self(self, player_id):
        self.team_inventory[player_id] = self.resources.copy()
        self.update_team_inventory(player_id)

    def get_team_missing_resource(self, level: int):
        level_name = f"LEVEL_{level + 1}"
        if level_name not in levels.__members__:
            return {}
        missing = {}
        total = self.team_inventory.get('total', {})
        requirements = getattr(levels, level_name).value
        for resource, needed in requirements.items():
            if resource == "Player":
                continue
            current = total.get(resource.lower(), 0)
            if current < needed:
                missing[resource.lower()] = needed - current
        return missing

    def update(self, resource: str, quantity: int):
        if resource in self.resources:
            self.resources[resource] = quantity

    def get(self, resource: str):
        return self.resources.get(resource)

    def get_missing_resource(self, level: int):
        level_name = f"LEVEL_{level + 1}"
        if level_name not in levels.__members__:
            return {}
        missing = {}
        requirements = getattr(levels, level_name).value
        for resource, needed in requirements.items():
            if resource == "Player":
                continue
            current = self.get(resource.lower())
            if current < needed:
                missing[resource.lower()] = needed - current
        return missing

    def farming_strategy(self, action, player):
        resource_scores = {}
        for resource_name in RESOURCE_RARITY.keys():
            if resource_name == "food":
                continue
            resource_tiles = action.find_resource(resource_name)
            if resource_tiles:
                best_tile = list(resource_tiles.keys())[0]
                best_count = resource_tiles[best_tile]
                total_quantity = sum(resource_tiles.values())
                score = player.get_resource_priority_score(resource_name, total_quantity)
                resource_scores[resource_name] = {
                    'score': score, 'total_quantity': total_quantity,
                    'best_tile': best_tile, 'best_count': best_count,
                    'is_priority': self.should_focus(resource_name, player),
                    'all_tiles': resource_tiles,
                }
        return resource_scores

    def should_focus(self, resource_name, player):
        return resource_name in self.get_missing_resource(player.level)
