
from entity import Entity


class Player(Entity):
    def __init__(self):
        super().__init__()
        self.surf.fill((255, 0, 255))
        self.speed = 1.0
