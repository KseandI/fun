
import pygame
import gmath

class Entity(pygame.sprite.Sprite):
    def __init__(self):
        super().__init__()
        self.size = gmath.Vector2(32, 32)
        self.surf = pygame.Surface((self.size.x, self.size.y))
        self.rect = self.surf.get_rect()
        self.position = gmath.Vector2()
        self.surf.fill((0, 0, 255))
        return

    def update(self):
        self.rect.update(self.position.x, self.position.y,
                         self.size.x, self.size.y)
        return

    def move(self, x = 0, y = 0):
        self.position.set(x, y)
        self.update()
        return

    def rel_move(self, x = 0, y = 0):
        self.position.add(x, y)
        self.update()
        return

    def rel_move_vec(self, vec):
        self.position += vec
        self.update()
        return

    def get_pos(self):
        return self.position

    def get_size(self):
        return self.size

    def get_status(self):
        return (self.position, self.size)

    def __str__(self):
        return f"<entity: {self.position.x}; {self.position.y}>"
    
