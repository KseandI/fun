
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

    def __add__(self, vec):
        if not isinstance(vec, Vector2):
            return self
        self.x += vec.x
        self.y += vec.y
        print(self)
        return self
