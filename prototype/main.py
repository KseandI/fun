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
        self.surface = pygame.display.set_mode(size)
        pygame.display.set_caption(name)
        return


class Entity():
    def __init__(self):
        return


class DrawEntity(Entity):
    def __init__(self):
        super().__init__()
        self.surface: pygame.Surface
        self.rect: pygame.rect
        self.init()
        return

    def init(self):
        self.surface = pygame.Surface((16, 16))
        self.surface.fill((0, 255, 0))
        self.rect = self.surface.get_rect()
        return


class Camera(Entity):
    def __init__(self):
        super().__init__()
        self.init()
        return

    def init(self):
        return


class Game():
    def __init__(self,
                 camera: Camera):
        self.camera: Camera
        self.is_running: bool
        self.init(camera)

    def init(self, camera):
        self.camera = camera
        self.is_running = True


def main():
    pman = PgameMan((640, 360), "fun")
    camera = Camera()
    ent = DrawEntity()
    game = Game(camera)

    while game.is_running:

        # code goes here
        
        for event in pygame.event.get():
            if event.type == QUIT:
                game.is_running = False


if __name__ == "__main__":
    main()


