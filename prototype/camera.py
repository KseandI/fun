
from entity import Entity
import gmath


class Camera():
    def __init__(self, surface, size = gmath.Vector2(200, 200)):
        self.position = gmath.Vector2(0, 0)
        self.size = size
        self.surface = surface
        self.rect = self.surface.get_rect()

    def update(self):
        self.rect.update(self.position.x, self.position.y,
                         self.size.x, self.size.y)
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

    def move_vec(self, vec: gmath.Vector2):
        if not isinstance(vec, gmath.Vector2):
            raise TypeError('tried to move_vec without vec')
        self.position = vec.copy()
        return
