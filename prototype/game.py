
import pygame
from pygame.locals import *

from entity import Entity
from player import Player
import gmath

class GameSystem:            
    def __init__(self):
        pygame.init()
        self.surface = pygame.display.set_mode((640, 360))
        self.kpressed = list()
        self.is_running = True
        self.rend_ents = list()
        self.player = Player()
        self.init()

        
    def loop(self):
        while self.is_running:
            self.event_check()
            if self.is_running == False:
                break

            self.player.rel_move_vec(gmath.Vector2(self.kpressed[K_f]-self.kpressed[K_b],
                                                   self.kpressed[K_n]-self.kpressed[K_p]))
            
            self.render()
    

    def event_check(self):
        for event in pygame.event.get():
            if event.type == QUIT:
                pygame.quit()
                self.is_running = False
        self.kpressed = pygame.key.get_pressed()
        if self.kpressed[K_q]:
            self.is_running = False
                    

    def render(self):
        self.surface.fill((0, 255, 255))

        for ent in self.rend_ents:
            self.surface.blit(ent.surf, ent.rect)

        pygame.display.update()


    def init(self):
        pygame.display.set_caption("fun")
        self.rend_ents.append(self.player)

        


