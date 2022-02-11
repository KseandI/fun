
from entity import Entity
from pygame.math import Vector2


class Camera():
    def __init__(self, surface, size = Vector2(200, 200)):
        self.position = Vector2(0, 0)
        self.size = size
        self.surface = surface
        self.rect = self.surface.get_rect()

    def update(self):
        self.rect = self.position
        return

    def render_entity(self, ent: Entity) -> int:
        if not isinstance(ent, Entity):
            return -1

        self.update()
        rect = ent.rect.copy()
        rect.x -= self.rect.x
        rect.y -= self.rect.y
        self.surface.blit(ent.surf, rect)
        return 0

    def move(self, x, y):
        self.position.x = x
        self.position.y = y
        self.update()
        return

    def move_vec(self, vec):
        if not isinstance(vec, Vector2):
            raise TypeError('tried to move_vec without vec')
        self.position = vec.copy()
        return
