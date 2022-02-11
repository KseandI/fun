
class Vector2:

    def __init__(self, x = 0, y = 0):
        self.x = x
        self.y = y
        return

    def set(self, x, y):
        self.x = x
        self.y = y
        return self

    def add(self, x, y):
        return Vector2(self.x + x, self.y + y)

    def copy(self):
        return Vector2(self.x, self.y)

    def __str__(self):
        return f"<{self.x}; {self.y}>"

    def __add__(self, vec):
        if not isinstance(vec, Vector2):
            raise TypeError('tried to add not vec to vec')
        return Vector2(self.x + vec.x, self.y + vec.y)

    def __mul__(self, val):
        return Vector2(self.x * val, self.y * val)

    def __truediv__(self, val):
        return Vector2(self.x / val, self.y / val)

    def __sub__(self, vec):
        if not isinstance(vec, Vector2):
            raise TypeError('tried to subtract value from vec2')
        return Vector2(self.x - vec.x, self.y - vec.y)
