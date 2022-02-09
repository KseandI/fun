
import pygame
from pygame.locals import *

class GameSystem:
    def loop(self):
        while self.is_running:
            self.event_check()
                    
            if self.is_running == False:
                break

            self.render()

    def event_check(self):
        for event in pygame.event.get():
            if event.type == QUIT:
                pygame.quit()
                self.is_running = False
                    

    def render(self):
        self.surface.fill((0, 255, 255))

        pygame.display.update()
                    
    def __init__(self):
        pygame.init()
        self.surface = pygame.display.set_mode((640, 360))
        self.is_running = True
        pygame.display.set_caption("fun")

        
