#!/usr/bin/env python3

##
## EPITECH PROJECT, 2026
## zappy
## File description:
## client - couche I/O non bloquante 
##

import socket
import select
from collections import deque


class Pending:
    __slots__ = ("command", "lines", "done", "result")
    def __init__(self, command):
        self.command = command
        self.lines = []
        self.done = False
        self.result = None


class SocketClient:
    MAX_IN_FLIGHT = 10

    def __init__(self, host, port):
        self.host = host
        self.port = port
        self.sock = None
        self._read_buffer = ""
        self._pending = deque()
        self._notifications = deque()
        self.dead = False

    def connect(self, team_name):
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sock.connect((self.host, self.port))
        self.sock.setblocking(False)

        welcome = self._read_line_blocking()
        if welcome != "WELCOME":
            raise ConnectionError(f"attendu WELCOME, recu {welcome!r}")
        self._write(team_name + "\n")

        client_slots = self._read_line_blocking()
        map_size = self._read_line_blocking()
        return client_slots, map_size

    def close(self):
        if self.sock:
            self.sock.close()
            self.sock = None

    def send_command(self, command):
        if self.dead or self.sock is None:
            return False
        if len(self._pending) >= self.MAX_IN_FLIGHT:
            return False
        self._pending.append(Pending(command))
        self._write(command + "\n")
        return True

    def request(self, command, timeout=1.0):
        if not self.send_command(command):
            return None
        entry = self._pending[-1]
        while not entry.done:
            if self.dead:
                return None
            self.poll(timeout)
        return entry.result

    def poll(self, timeout=0.0):
        if self._drain_socket(timeout):
            self._process_buffer()

    def next_notification(self):
        if self._notifications:
            return self._notifications.popleft()
        return None

    def has_notifications(self):
        return bool(self._notifications)

    def in_flight(self):
        return len(self._pending)

    def _write(self, data):
        self.sock.sendall(data.encode())

    def _drain_socket(self, timeout):
        readable, _, _ = select.select([self.sock], [], [], timeout)
        if not readable:
            return False
        chunk = self.sock.recv(4096).decode()
        if not chunk: 
            self.dead = True
            self._notifications.append(("dead", None))
            return False
        self._read_buffer += chunk
        return True

    def _read_line_blocking(self, timeout=5.0):
        """Lit une ligne complete, utilise uniquement pendant le handshake."""
        while "\n" not in self._read_buffer:
            readable, _, _ = select.select([self.sock], [], [], timeout)
            if not readable:
                raise TimeoutError("timeout pendant le handshake")
            chunk = self.sock.recv(4096).decode()
            if not chunk:
                raise ConnectionError("serveur ferme pendant le handshake")
            self._read_buffer += chunk
        line, self._read_buffer = self._read_buffer.split("\n", 1)
        return line.strip()

    def _process_buffer(self):
        while "\n" in self._read_buffer:
            line, self._read_buffer = self._read_buffer.split("\n", 1)
            self._route(line.strip())

    def _route(self, line):
        if not line:
            return
        if line == "dead":
            self.dead = True
            self._notifications.append(("dead", None))
            return
        if line.startswith("message"):
            head, _, text = line.partition(",")
            try:
                direction = int(head.split()[1])
            except (IndexError, ValueError):
                direction = 0
            self._notifications.append(("message", (direction, text.strip())))
            return
        if line.startswith("eject"):
            try:
                direction = int(line.split(":", 1)[1])
            except (IndexError, ValueError):
                direction = 0
            self._notifications.append(("eject", direction))
            return

        head = self._pending[0] if self._pending else None
        own_incantation = head is not None and head.command == "Incantation"

        if line == "Elevation underway":
            if own_incantation:
                head.lines.append(line)
                return
            self._notifications.append(("elevation_start", None))
            return
        if line.startswith("Current level:"):
            if not own_incantation:
                try:
                    level = int(line.split(":", 1)[1])
                except (IndexError, ValueError):
                    level = None
                self._notifications.append(("level", level))
                return
        if head is None:
            return     
        head.lines.append(line)
        head.result = line
        head.done = True
        self._pending.popleft()
