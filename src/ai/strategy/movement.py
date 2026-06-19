#!/usr/bin/env python3

##
## EPITECH PROJECT, 2026
## zappy
## File description:
## movement - calcul des chemins vers les cases de vision
##


class Movement:
    def __init__(self, level):
        self.movement = {}
        self.level = level
        self.get_all()

    def reachable(self):
        tiles = {0: (0, 0)}
        tile_number = 1
        for distance in range(1, self.level + 1):
            width = 2 * distance + 1
            start_x = -distance
            for temp in range(width):
                x = start_x + temp
                y = distance
                tiles[tile_number] = (x, y)
                tile_number += 1
        return tiles

    def path_to_tile(self, x, y):
        moves = []
        for _ in range(y):
            moves.append("Forward")
        if x < 0:
            moves.append("Left")
            for _ in range(abs(x)):
                moves.append("Forward")
            moves.append("Right")
        elif x > 0:
            moves.append("Right")
            for _ in range(x):
                moves.append("Forward")
            moves.append("Left")
        return moves

    def get_all(self):
        all_tiles = self.reachable()
        all_moves = {}
        for tile_number, (x, y) in all_tiles.items():
            if tile_number == 0:
                continue
            all_moves[tile_number] = self.path_to_tile(x, y)
        self.movement = all_moves

    def get_moves(self, vision_tile_number):
        calculated_tile_number = vision_tile_number - 1
        if calculated_tile_number not in self.movement:
            return None
        return self.movement[calculated_tile_number]

    def center_boss(self, direction):
        if direction == "0":
            return []
        turn_toward = {
            "1": [], "2": ["Left"], "3": ["Left"], "4": ["Left", "Left"],
            "5": ["Left", "Left"], "6": ["Right", "Right"], "7": ["Right"],
            "8": ["Right"],
        }
        if direction not in turn_toward:
            return None
        return turn_toward[direction] + ["Forward"]
