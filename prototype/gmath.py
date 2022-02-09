
import math

class Vector2:

    def __init__(self, x = 0, y = 0):
        self.x = x
        self.y = y

    def set(self, x, y):
        self.x = x
        self.y = y

    def add(self, x, y):
        self.x += x
        self.y += y

    def __str__(self):
        return f"<{self.x}; {self.y}>"
