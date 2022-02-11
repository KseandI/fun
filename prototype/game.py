
import pygame
from pygame.locals import *
from pygame.math import Vector2

from entity import Entity
from player import Player
from camera import Camera


class GameSystem:
    def __init__(self):
        pygame.init()
        self.window_size = Vector2(640, 360)
        self.surface = pygame.display.set_mode((self.window_size.x,
                                                self.window_size.y))
        self.kpressed = list()
        self.is_running = True
        self.rend_ents = list()
        self.player = Player()
        self.camera = Camera(self.surface, self.window_size)
        self.init()

    def init(self):
        pygame.display.set_caption("fun")
        self.rend_ents.append(self.player)
        self.rend_ents.append(Entity())
        self.player.speed = float.fromhex('0x1')

    def loop(self):
        while self.is_running:
            self.event_check()
            if not self.is_running:
                break

            rel_x = self.kpressed[K_f] - self.kpressed[K_b]
            rel_y = self.kpressed[K_n] - self.kpressed[K_p]
            relative_movement = (Vector2(rel_x, rel_y) * self.player.speed)
            self.player.rel_move_vec(relative_movement)

            self.camera.move_vec(self.player.position
                                 - (self.camera.size / 2)
                                 + (self.player.size / 2))

            self.render()

    def event_check(self):
        for event in pygame.event.get():
            if event.type == QUIT:
                pygame.quit()
                self.is_running = False
                return
        self.kpressed = pygame.key.get_pressed()
        if self.kpressed[K_q]:
            self.is_running = False
            return
        return

    def render(self):
        self.surface.fill((0, 255, 255))

        for ent in self.rend_ents:
            self.camera.render_entity(ent)

        pygame.display.update()
