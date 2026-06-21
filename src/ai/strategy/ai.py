#!/usr/bin/env python3

##
## EPITECH PROJECT, 2026
## zappy
## File description:
## ai - orchestration : connexion, boucle principale, gestion des workers
##

import os
import sys
import time
import queue
import subprocess

from Client import SocketClient
from config import Mode, TARGET_PLAYERS, TEAMMATE_TTL
from player import Player
from action import Action
from decision import Decision

ENTRY_POINT = os.path.join(os.path.dirname(os.path.abspath(__file__)), "ai_algo.py")


class AI:
    def __init__(self, team_name, host, port):
        self.team_name = team_name
        self.player = Player(team_name)
        self.client = SocketClient(host, port)
        self.action = Action(self)
        self.decision = Decision(self.player, Mode.SURVIVAL, self.action)
        self.running = True
        self.broadcast_queue = queue.Queue()
        self.map_x = 0
        self.map_y = 0
        self.confirmed_players = set()
        self.max_client = 0
        self.last_seen = {}

    def connecting(self):
        client_slots, map_size = self.client.connect(self.team_name)
        self.map_x, self.map_y = (int(v) for v in map_size.split())
        if not self.player.boss:
            return
        self.max_client = int(client_slots.strip())
        slots_remaining = int(self.action.execute_action("Connect_nbr"))
        current_players = self.max_client - slots_remaining
        if current_players < TARGET_PLAYERS:
            workers_needed = TARGET_PLAYERS - current_players
            slots_needed = TARGET_PLAYERS - self.max_client
            for _ in range(max(0, slots_needed)):
                self.action.fork_player()
            self.launch_workers(min(workers_needed, max(slots_remaining, 0)
                                    + max(0, slots_needed)))

    def _prune_dead_teammates(self, now):
        self_id = str(self.player.player_id)
        dead = [pid for pid, ts in self.last_seen.items()
                if now - ts > TEAMMATE_TTL]
        for pid in dead:
            self.last_seen.pop(pid, None)
            if pid != self_id:
                self.player.inventory.team_inventory.pop(pid, None)
            self.confirmed_players.discard(pid)
        if dead:
            self.player.inventory.recompute_total()

    def maintain_population(self):
        self._prune_dead_teammates(time.time())
        living = len([pid for pid in self.player.inventory.team_inventory
                      if pid != "total"])
        if living >= TARGET_PLAYERS:
            return
        missing = TARGET_PLAYERS - living
        try:
            slots_remaining = int(self.action.execute_action("Connect_nbr"))
        except (TypeError, ValueError):
            slots_remaining = 0
        for _ in range(max(0, missing - slots_remaining)):
            self.action.fork_player()
        self.launch_workers(missing)

    def launch_workers(self, count):
        for _ in range(max(0, count)):
            subprocess.Popen([
                sys.executable, ENTRY_POINT,
                "-p", str(self.client.port),
                "-n", self.team_name,
                "-h", self.client.host,
                "--worker",
            ])

    def parse_broadcast(self, message):
        try:
            if message.startswith("message"):
                head, _, text = message.partition(",")
                direction = int(head.split()[1])
                self.action.parse_team_broadcast(text.strip(), direction)
        except Exception:
            pass

    def process_messages(self):
        while not self.broadcast_queue.empty():
            self.parse_broadcast(self.broadcast_queue.get())

    def my_ai(self):
        try:
            self.connecting()
            while self.running and not self.client.dead:
                self.client.poll(0.0)
                self.action._drain_notifications()
                self.process_messages()
                self.decision.set_next()
        except KeyboardInterrupt:
            pass
        finally:
            self.client.close()
