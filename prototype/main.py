#!/usr/bin/env python

import pygame
from pygame.locals import *
from pygame.math import Vector2


class PgameMan:
    def __init__(self,
                 size: (int, int) = (640, 360),
                 name: str = "window"):
        """ for variable declorations """
        self.surface: pygame.Surface
        self.init(size, name)

        
    def init(self, size, name):
        pygame.init()
        surface = pygame.display.set_mode(size)
        pygame.display.set_caption(name)


class Entity():
    def __init__(self):
        pass

class Camera(Entity):
    def __init__(self):
        super().__init__()
    


def main():
    pman = PgameMan((640, 360), "fun")


if __name__ == "__main__":
    main()
