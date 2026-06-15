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
import socket
from typing import Optional

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
    FORWARD = 0
    LEFT = 1
    BACKWARD = 2
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


class ZappyClient:
    def __init__(self, host: str, port: int, team_name: str):
        self.host = host
        self.port = port
        self.team_name = team_name
        self.socket: Optional[socket.socket] = None
        self.client_num: Optional[int] = None
        self.world_width: Optional[int] = None
        self.world_height: Optional[int] = None
        self.buffer = ""

    def connect(self) -> bool:
        try:
            self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.socket.connect((self.host, self.port))
            print(f"[CONNECTION] Connected to {self.host}:{self.port}")
            
            welcome = self.receive_line()
            if welcome != "WELCOME":
                print(f"[ERROR] Expected WELCOME, got: {welcome}")
                return False
            
            self.send_line(self.team_name)
            
            client_num_str = self.receive_line()
            try:
                self.client_num = int(client_num_str)
            except ValueError:
                print(f"[ERROR] Invalid CLIENT-NUM: {client_num_str}")
                return False
            
            dimensions = self.receive_line().split()
            if len(dimensions) != 2:
                print(f"[ERROR] Expected 'X Y', got: {dimensions}")
                return False

            try:
                self.world_width = int(dimensions[0])
                self.world_height = int(dimensions[1])
            except ValueError:
                print(f"[ERROR] Invalid dimensions: {dimensions}")
                return False
            
            print(f"[INFO] Client #{self.client_num}")
            print(f"[INFO] World size: {self.world_width}x{self.world_height}")
            return True

        except Exception as e:
            print(f"[ERROR] Connection failed: {e}")
            return False
    
    def send_command(self, command: str) -> None:
        if not self.socket:
            print("[ERROR] Not connected to server")
            return
        try:
            self.socket.sendall(f"{command}\n".encode())
            print(f"[SEND] {command}")
        except Exception as e:
            print(f"[ERROR] Failed to send command: {e}")
    
    def receive_response(self, timeout: float = 1.0) -> Optional[str]:
        if not self.socket:
            return None
        
        try:
            self.socket.settimeout(timeout)
            
            while True:
                if "\n" in self.buffer:
                    message, self.buffer = self.buffer.split("\n", 1)
                    return message.strip()
                
                data = self.socket.recv(4096).decode()
                if not data:
                    return None
                self.buffer += data
    
        except socket.timeout:
            return None
        except Exception as e:
            print(f"[ERROR] Failed to receive: {e}")
            return None

    def receive_line(self) -> str:
        line = ""
        while True:
            char = self.socket.recv(1).decode()
            if char == "\n":
                return line
            line += char
    
    def send_line(self, line: str) -> None:
        self.socket.sendall(f"{line}\n".encode())

class SurvivalAI:
    def __init__(self, client: ZappyClient):
        self.client = client
        self.inventory = Inventory()
        self.state = State.SURVIVE
        self.current_direction = Direction.FORWARD
        self.position_x = 0
        self.position_y = 0
        self.level = 1

    def act(self) -> None:
        self.state = self.update_state()
        self.client.send_command("Look")
        look_response = self.client.receive_response()

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

    def inventory(self) -> None:
        self.client.send_command("Inventory")
        response = self.client.receive_response()
        self.current_direction = Direction((self.current_direction.value - 1) % 4)
    

def parser(argv):
    port = None
    team = None
    host = "localhost"

    try:
        opts, args = getopt.getopt(argv, "p:n:h:", ["help"])
    except getopt.GetoptError:
        print("USAGE: ./zappy_ai -p port -n name -h machine")
        sys.exit(84)

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

    client = ZappyClient(host, port, team)
    if not client.connect():
        sys.exit(1)
    
    while True:
        None

if __name__ == "__main__":
    main(sys.argv[1:])