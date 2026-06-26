##
## EPITECH PROJECT, 2026
## zappy
## File description:
## decision - machine a etats de la strategie (boss / worker)
##

import time

from strategy.Utils import (IncantationState, levels, players_needed, REPOP_PERIOD,
                    FOOD_SURVIVAL_TARGET, FOOD_RECOVERY_TARGET, TARGET_PLAYERS)
from pathfinding.Movement import Movement


class Decision:
    BROADCAST_PERIOD = 5
    GATHER_WAIT = 1.0
    MAX_RECOVERY_TIME = 15
    GATHER_STABLE_WAIT = 5

    def __init__(self, player, mode, action):
        self.player = player
        self.current_mode = mode
        self.action = action
        self.tick = 0
        self.recovery_start = 0
        self._gather_peak = 0
        self._gather_peak_time = 0

    def _should_abort_gathering(self):
        if self.player.incantation_state not in (
                IncantationState.GATHERING_PLAYERS,
                IncantationState.MASTER_DROPPING):
            return False
        return self.player.is_very_hungry()

    def set_next(self):
        self.tick += 1
        self.action.fetch_inventory(self.player)
        if self.tick % self.BROADCAST_PERIOD == 0:
            self.action.broadcast_inventory()

        if self.player.incantation_state == IncantationState.RECOVERY:
            if self.player.boss:
                return self.handle_recovery_phase()
            return self.handle_worker_recovery()
        if self.player.incantation_state != IncantationState.IDLE:
            if self._should_abort_gathering():
                self.reset_incantation_state()
                return self.survival()
            return self.handle_incantation_state()

        if (self.player.boss and self.tick % REPOP_PERIOD == 0
                and not self.player.is_hungry()):
            self.action.ai.maintain_population()

        started = self._try_start_team_incantation()
        if started is not None:
            return started

        if not self.player.can_subsist:
            if self.player.is_hungry() or self.player.is_very_hungry():
                return self.survival()
            if self.player.get_food_level() >= self.player.food_objective():
                self.player.can_subsist = True
        return self.survival()

    def _try_start_team_incantation(self):
        if not (self.player.boss and self.player.level < 8
                and self.player.get_food_level() >= 30
                and self.player.can_team_upgrade(self.player.level)):
            return None
        if not self.player.is_immobile:
            self.player.set_immobile("incantation")
        self.player.incantation_state = IncantationState.GATHERING_PLAYERS
        self.action.ai.confirmed_players.clear()
        self._gather_peak = 0
        self._gather_peak_time = time.time()
        return self.gather_team_for_incantation()

    def players_rasso(self):
        minimum = players_needed(self.player.level)
        alive = len([p for p in self.player.inventory.team_inventory
                     if p != "total"])
        return max(minimum, min(TARGET_PLAYERS, alive))

    def _group_ready(self, players_on_tile, required):
        return players_on_tile >= required

    def gather_team_for_incantation(self):
        vision = self.action.look_around_with_player()
        players_on_tile = vision[1].count("player") if 1 in vision else 0
        required = self.players_rasso()
        minimum = players_needed(self.player.level)

        now = time.time()
        if players_on_tile > self._gather_peak:
            self._gather_peak = players_on_tile
            self._gather_peak_time = now
        stable = (players_on_tile >= minimum
                  and now - self._gather_peak_time > self.GATHER_STABLE_WAIT)

        self.action.broadcast_gather_for_incantation()
        if self._group_ready(players_on_tile, required) or stable:
            self.action.broadcast_check_ready()
            self.player.incantation_state = IncantationState.MASTER_DROPPING
        elif 1 in vision and 'food' in vision[1]:
            self.action.collect_resources('food', vision[1].count('food'), self.player)
        else:
            time.sleep(self.GATHER_WAIT)
        return True

    def drop_resources_and_coordinate(self):
        vision = self.action.look_around_with_player()
        players_on_tile = vision[1].count("player") if 1 in vision else 0
        required = self.players_rasso()

        if self._group_ready(players_on_tile, required):
            if self.drop_resources_for_team_incantation() is False:
                self.reset_incantation_state()
                return False
            self.action.broadcast_start_team_incantation()
            self.player.incantation_state = IncantationState.EXECUTING
            return self.execute_team_incantation()
        self.player.incantation_state = IncantationState.GATHERING_PLAYERS
        return self.gather_team_for_incantation()

    def execute_team_incantation(self):
        result = self.action.execute_action("Incantation")
        if result and result.startswith("Current level:"):
            try:
                self.player.level = int(result.split(":")[1])
            except (IndexError, ValueError):
                self.player.level += 1
            self.player.movement = Movement(self.player.level)
            self.player.inventory.team_inventory.clear()
            self.action.ai.confirmed_players.clear()
            self.player.set_mobile()
            self.player.can_subsist = False
            self.player.incantation_state = (
                IncantationState.IDLE if self.player.level >= 8
                else IncantationState.RECOVERY)
            return True
        self.reset_incantation_state()
        return False

    def _recovery_elapsed(self):
        if self.recovery_start == 0:
            self.recovery_start = time.time()
        return time.time() - self.recovery_start

    def _end_recovery(self):
        self.player.incantation_state = IncantationState.IDLE
        self.player.can_subsist = True
        self.recovery_start = 0

    def handle_recovery_phase(self):
        target_food = FOOD_RECOVERY_TARGET
        elapsed = self._recovery_elapsed()
        if elapsed > self.MAX_RECOVERY_TIME:
            self._end_recovery()
            return True
        if (self.player.get_food_level() >= target_food
                and self.player.can_team_upgrade(self.player.level)):
            self._end_recovery()
            return True
        return self.collect_food_strategy(target_food)

    def handle_worker_recovery(self):
        target_food = FOOD_RECOVERY_TARGET
        elapsed = self._recovery_elapsed()
        if elapsed > self.MAX_RECOVERY_TIME or self.player.get_food_level() >= target_food:
            self._end_recovery()
            return True
        return self.collect_food_strategy(target_food)

    def reset_incantation_state(self):
        self.player.incantation_state = IncantationState.IDLE
        self.action.ai.confirmed_players.clear()
        self.player.set_mobile()
        if self.player.boss:
            self.action.broadcast_message(
                f"{self.action.ai.team_name}|RESET|{self.player.player_id}")

    def count_resource_on_ground_with_players(self, resource_name):
        vision = self.action.look_around_with_player()
        if vision and 1 in vision:
            return vision[1].count(resource_name)
        return 0

    def drop_resources_for_team_incantation(self):
        level_name = f"LEVEL_{self.player.level + 1}"
        if level_name not in levels.__members__:
            return False
        requirements = getattr(levels, level_name).value
        if self.player.inventory.get_missing_resource(self.player.level):
            return False
        for resource, needed in requirements.items():
            if resource == "Player":
                continue
            resource_lower = resource.lower()
            while self.count_resource_on_ground_with_players(resource_lower) < needed:
                if self.player.inventory.get(resource_lower) <= 0:
                    return False
                if self.action.execute_action(f"Set {resource_lower}") == "ok":
                    self.player.inventory.resources[resource_lower] -= 1
                else:
                    return False
        for resource, needed in requirements.items():
            if resource == "Player":
                continue
            if self.count_resource_on_ground_with_players(resource.lower()) < needed:
                return False
        return True

    def handle_incantation_state(self):
        state = self.player.incantation_state

        if state == IncantationState.GATHERING_PLAYERS:
            if self.player.boss:
                return self.gather_team_for_incantation()
            vision = self.action.look_around_with_player()
            if 1 in vision and vision[1].count("player") >= 2:
                self.action.broadcast_worker_arrived()
                self.player.incantation_state = IncantationState.MASTER_DROPPING
            return True

        if state == IncantationState.MASTER_DROPPING:
            if self.player.boss:
                return self.drop_resources_and_coordinate()
            vision = self.action.look_around_with_player()
            if 1 in vision and 'food' in vision[1]:
                self.action.collect_resources('food', vision[1].count('food'),
                                              self.player)
            else:
                time.sleep(0.5)
            return True

        if state == IncantationState.EXECUTING:
            if self.player.boss:
                return self.execute_team_incantation()
            return True
        return self.farm()

    def survival(self):
        target_food = FOOD_SURVIVAL_TARGET
        if self.player.is_very_hungry():
            return self.collect_food_strategy(target_food)
        if self.player.get_food_level() < self.player.food_objective():
            return self.collect_food_strategy(target_food)
        self.collect_stones()
        if self.player.get_food_level() >= target_food:
            return True
        return self.collect_food_strategy(target_food)

    def collect_food_strategy(self, target_amount):
        vision = self.action.look_around_with_player()
        if 1 in vision and 'food' in vision[1]:
            self.action.collect_all_resources_on_tile(self.player)
            if self.player.get_food_level() >= target_amount:
                return True
        food_tiles = self.action.find_resource("food")
        if not food_tiles:
            self.action.explore_randomly()
            return False
        closest_tile = min(food_tiles.keys())
        if closest_tile > 1 and not self.action.move_to_tile(closest_tile):
            self.action.explore_randomly()
            return False
        return len(self.action.collect_all_resources_on_tile(self.player)) > 0

    def collect_stones(self):
        vision = self.action.look_around()
        if not vision:
            return {}
        stones = ['linemate', 'deraumere', 'sibur', 'mendiane', 'phiras', 'thystame']
        stones_collected = {}
        if 1 in vision:
            for stone in stones:
                if stone in vision[1]:
                    count = self.action.collect_resources(
                        stone, vision[1].count(stone), self.player)
                    if count > 0:
                        stones_collected[stone] = count
        if stones_collected:
            return stones_collected
        for tile_num in sorted(vision.keys()):
            if tile_num == 1:
                continue
            for stone in stones:
                if stone in vision[tile_num]:
                    if self.action.move_to_tile(tile_num):
                        count = self.action.collect_resources(
                            stone, vision[tile_num].count(stone), self.player)
                        if count > 0:
                            stones_collected[stone] = count
                    return stones_collected
        return stones_collected

    def farm(self):
        resource_scores = self.player.inventory.farming_strategy(
            self.action, self.player)
        if not resource_scores:
            self.action.explore_randomly()
            return
        best_resource, best_tile = self.action.find_best_resource_tile(resource_scores)
        if best_resource and best_tile:
            self.action.move_to_tile(best_tile)
            self.action.collect_all_resources_on_tile(self.player)
        else:
            self.action.explore_randomly()
