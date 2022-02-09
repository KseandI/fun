
import pygame
from pygame.locals import *

from entity import Entity
from player import Player

class GameSystem:            
    def __init__(self):
        pygame.init()
        self.surface = pygame.display.set_mode((640, 360))
        self.is_running = True
        self.rend_ents = list()
        self.player = Player()
        self.init()

        
    def loop(self):
        while self.is_running:
            self.event_check()
            if self.is_running == False:
                break

            self.player.rel_move(self.player.speed.x, 0)
            pos = self.player.get_pos()
            if pos.x >= 640 or pos.x < 0:
                self.player.speed.x *= -1
            
            self.render()
    

    def event_check(self):
        for event in pygame.event.get():
            if event.type == QUIT:
                pygame.quit()
                self.is_running = False
                    

    def render(self):
        self.surface.fill((0, 255, 255))

        for ent in self.rend_ents:
            self.surface.blit(ent.surf, ent.rect)

        pygame.display.update()


    def init(self):
        pygame.display.set_caption("fun")
        self.rend_ents.append(self.player)

        


