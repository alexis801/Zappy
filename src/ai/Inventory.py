##
## EPITECH PROJECT, 2026
## Zappy
## File description:
## inventory
##

from dataclasses import dataclass

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
