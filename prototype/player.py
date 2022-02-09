
import pygame

from entity import Entity
import gmath

class Player(Entity):
    def __init__(self):
        super().__init__()
        self.surf.fill((255, 0, 255))
        self.speed = gmath.Vector2(0.3, 0)
