##
## EPITECH PROJECT, 2026
## Zappy
## File description:
## AiClient
##

from typing import Optional
import socket

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
