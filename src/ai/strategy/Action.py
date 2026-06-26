##
## EPITECH PROJECT, 2026
## zappy
## File description:
## action - couche d'actions de jeu (Look, Take, Broadcast, deplacements)
##

import time
import random

from strategy.Utils import (IncantationState, resource_map, broadcast_key,
                            encrypt_message, decrypt_message)
from pathfinding.Movement import Movement

class Action:
    def __init__(self, ai):
        self.ai = ai
        self.client = ai.client
        self.player = ai.player

    def can_move(self):
        return not self.player.is_immobile

    def _drain_notifications(self):
        note = self.client.next_notification()
        while note is not None:
            kind, payload = note
            if kind == "message":
                direction, text = payload
                self.ai.broadcast_queue.put(f"message {direction}, {text}")
            elif kind == "dead":
                self.ai.running = False
            elif kind == "level" and payload is not None:
                self.player.level = payload
                self.player.movement = Movement(payload)
                self.player.incantation_state = IncantationState.RECOVERY
            elif kind == "eject":
                if self.player.incantation_state != IncantationState.IDLE:
                    self.player.incantation_state = IncantationState.IDLE
                    self.player.set_mobile()
                    self.ai.confirmed_players.clear()
            note = self.client.next_notification()

    def execute_action(self, action, parameter=""):
        command = action + (f" {parameter}" if parameter else "")
        self._drain_notifications()
        response = self.client.request(command)
        self._drain_notifications()
        return response

    def broadcast_message(self, message):
        payload = encrypt_message(message, broadcast_key(self.ai.team_name))
        response = self.execute_action("Broadcast", payload)
        return response is not None and response != "ko"

    def move_to_tile(self, tile_number):
        if not self.can_move():
            return False
        moves = self.player.movement.get_moves(tile_number)
        if moves is None:
            return False
        for action in moves:
            response = self.execute_action(action)
            if response is None or response == "ko":
                return None
        return True

    def fork_player(self):
        return self.execute_action("Fork") == "ok"

    def find_resource(self, resource_name):
        vision = self.look_around()
        if not vision:
            return {}
        resource_tiles = {}
        for tile, content in vision.items():
            if resource_name in content:
                resource_tiles[tile] = content.count(resource_name)
        return dict(sorted(resource_tiles.items(), key=lambda kv: kv[1], reverse=True))

    def look_around_with_player(self):
        return self._parse_look(keep_players=True)

    def look_around(self):
        return self._parse_look(keep_players=False)

    def _parse_look(self, keep_players):
        raw = self.execute_action("Look")
        if raw is None:
            return {}
        raw = raw.replace('[', '').replace(']', '').strip('"')
        result = {}
        for index, segment in enumerate(raw.split(',')):
            tile_number = index + 1
            content = segment.split() if segment.strip() else [""]
            if not keep_players:
                content = [item for item in content if item != "player"] or [""]
            result[tile_number] = content
        return result

    def fetch_inventory(self, player):
        response = self.execute_action("Inventory")
        if response:
            player.update_inventory(response)
            player.inventory.register_self(str(player.player_id))

    def collect_resources(self, resource_name, quantity, player):
        collected = 0
        while collected < quantity:
            if self.execute_action(f"Take {resource_name}") == "ok":
                collected += 1
                player.inventory.resources[resource_name] += 1
            else:
                break
        return collected

    def collect_all_resources_on_tile(self, player):
        vision = self.look_around_with_player()
        if not vision or 1 not in vision:
            return {}
        collected = {}
        for item in vision[1]:
            if item == "player" or not item:
                continue
            count = self.collect_resources(item, 999999, player)
            if count > 0:
                collected[item] = count
        return collected

    def find_best_resource_tile(self, resource_scores):
        if not resource_scores:
            return None, None
        sorted_resources = sorted(
            resource_scores.items(),
            key=lambda item: (item[1]['is_priority'], item[1]['score']),
            reverse=True)
        best_resource, best_data = sorted_resources[0]
        return best_resource, best_data['best_tile']

    def explore_randomly(self):
        if not self.can_move():
            return False
        if random.random() < 0.3:
            self.execute_action(random.choice(["Left", "Right"]))
        self.execute_action("Forward")
        return True

    def broadcast_inventory(self):
        inv = self.player.inventory.resources
        compact = [f"{abbrev}{inv.get(res, 0)}"
                   for res, abbrev in resource_map.items()
                   if res != "food" and inv.get(res, 0) > 0]
        inventory_str = ",".join(compact) if compact else "0"
        food = inv.get("food", 0)
        message = (f"{self.ai.team_name}|I|{self.player.player_id}|"
                   f"{self.player.level}|{inventory_str}|{food}|{self.ai.status[0]}")
        self.broadcast_message(message)

    def broadcast_gather_for_incantation(self):
        return self.broadcast_message(
            f"{self.ai.team_name}|G|{self.player.player_id}|{self.player.level}")

    def broadcast_worker_arrived(self):
        return self.broadcast_message(
            f"{self.ai.team_name}|ARRIVED|{self.player.player_id}|{self.player.level}")

    def broadcast_check_ready(self):
        return self.broadcast_message(
            f"{self.ai.team_name}|C|{self.player.player_id}")

    def broadcast_start_team_incantation(self):
        return self.broadcast_message(
            f"{self.ai.team_name}|S|{self.player.player_id}")

    def broadcast_recovery_start(self):
        return self.broadcast_message(
            f"{self.ai.team_name}|RECOVERY|{self.player.player_id}|{self.player.level}")

    def move_to_sound_source(self, direction):
        if not self.can_move():
            return False
        moves = self.player.movement.center_boss(str(direction))
        if not moves:
            return False
        for move in moves:
            if self.execute_action(move) == "ko":
                return False
        vision = self.look_around_with_player()
        if 1 in vision and vision[1].count("player") >= 2:
            self.player.incantation_state = IncantationState.MASTER_DROPPING
            self.broadcast_worker_arrived()
            return True
        return False

    def parse_team_broadcast(self, message, direction):
        plain = decrypt_message(message, broadcast_key(self.ai.team_name))
        if plain is None:
            return False
        try:
            parts = plain.split("|")
            if len(parts) < 2 or parts[0] != self.ai.team_name:
                return False
            broadcast_type = parts[1]
            player_id = parts[2] if len(parts) > 2 else None

            if broadcast_type == "I":
                food_amount = int(parts[5])
                inventory_compact = parts[4]
                inventory = dict.fromkeys(
                    ["linemate", "deraumere", "sibur", "mendiane",
                     "phiras", "thystame"], 0)
                inventory["food"] = food_amount
                if inventory_compact != "0":
                    for item in inventory_compact.split(","):
                        if len(item) >= 2:
                            for res_name, res_code in resource_map.items():
                                if res_code == item[0]:
                                    inventory[res_name] = int(item[1:])
                                    break
                self.player.inventory.team_inventory[player_id] = inventory
                self.player.inventory.update_team_inventory(player_id)
                self.ai.last_seen[player_id] = time.time()
                if len(parts) > 3:
                    self.ai.teammate_levels[player_id] = int(parts[3])
            elif broadcast_type == "S" and not self.player.boss:
                self.player.incantation_state = IncantationState.EXECUTING
            elif broadcast_type == "RESET" and not self.player.boss:
                self.player.incantation_state = IncantationState.IDLE
            elif broadcast_type == "G" and not self.player.boss:
                boss_level = int(parts[3]) if len(parts) > 3 else self.player.level
                if boss_level != self.player.level:
                    return True
                if self.player.incantation_state == IncantationState.MASTER_DROPPING:
                    return True
                self.player.incantation_state = IncantationState.GATHERING_PLAYERS
                if direction == 0:
                    self.player.incantation_state = IncantationState.MASTER_DROPPING
                    self.broadcast_worker_arrived()
                else:
                    self.move_to_sound_source(direction)
            elif broadcast_type == "ARRIVED" and self.player.boss:
                self.ai.confirmed_players.add(player_id)
            elif broadcast_type == "RECOVERY" and not self.player.boss:
                self.player.can_subsist = False
            elif broadcast_type == "PRESENT" and self.player.boss:
                self.ai.confirmed_players.add(player_id)
            return True
        except Exception:
            return False
