#!/usr/bin/env python

import pygame
from pygame.locals import *
from pygame.math import Vector2


class PgameMan:
    def __init__(self,
                 size: (int, int) = (640, 360),
                 name: str = "window"):
        self.surface: pygame.Surface
        
        pygame.init()
        self.surface = pygame.display.set_mode(size)
        pygame.display.set_caption(name)

    def clear(self,
              color: (int, int, int)):
        self.surface.fill(color)
        return

    def update(self):
        pygame.display.update()
        return

    def get_keys(self):
        return pygame.key.get_pressed()

    
class Entity():
    def __init__(self):
        return


class DrawEntity(Entity):
    def __init__(self):
        super().__init__()
        self.surface: pygame.Surface
        self.position: Vector2
        
        self.surface = pygame.Surface((16, 16))
        self.surface.fill((0, 255, 0))
        self.position = Vector2((0, 0))
        return

    
class Player(DrawEntity):
    def __init__(self):
        super().__init__()
        self.speed: float

        self.speed = 0.2
        return


class Camera(Entity):
    def __init__(self, surface):
        super().__init__()
        self.surface: pygame.Surface
        self.position: Vector2
        
        self.surface = surface
        self.position = Vector2((0, 0))
        return

    def render(self,
               entity: DrawEntity):
        self.surface.blit(entity.surface, entity.position - self.position)
        return


class Game():
    def __init__(self,
                 camera: Camera):
        self.camera: Camera
        self.is_running: bool
        self.render_stack: list

        self.camera = camera
        self.render_stack = list()
        self.is_running = True
        return

    def add_render(self,
                   entity: DrawEntity):
        self.render_stack.append(entity)
        return


def main():
    pman = PgameMan((640, 360), "fun")
    camera = Camera(pman.surface)
    player = Player()
    game = Game(camera)

    game.add_render(player)

    while game.is_running:

        ## logic ##

        pkeys = pman.get_keys()
        wish_dir = (Vector2(pkeys[K_f]-pkeys[K_b],
                            pkeys[K_n]-pkeys[K_p])
                    * player.speed)
        player.position += wish_dir

        ## render ##

        pman.clear((0, 255, 255))

        for ent in game.render_stack:
            camera.render(ent)

        pman.update()
        
        for event in pygame.event.get():
            if event.type == QUIT:
                game.is_running = False


if __name__ == "__main__":
    main()











