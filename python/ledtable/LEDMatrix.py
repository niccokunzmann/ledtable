from abc import ABC, abstractmethod

class LEDMatrix(ABC):

    @abstractmethod
    def __init__(self, width, height):
        pass

    @abstractmethod
    def _width(self):
        pass

    def _set_width(self, width):
        raise NotImplementedError("The width can not be changed")

    @abstractmethod
    def _height(self):
        pass

    def _set_height(self, height):
        raise NotImplementedError("The height can not be changed")

    @abstractmethod
    def _fill_pixel(self, x, y, color):
        pass
    
    @property
    def width(self):
        return self._width()
    @width.setter
    def width(self, width):
        self._set_width(width)

    @property
    def height(self):
        return self._height()
    @height.setter
    def height(self, height):
        self._set_height(height)
    
    @property
    def dimensions(self):
        return self.width, self.height
    @dimensions.setter
    def dimensions(self, dimensions):
        self.width, self.height = dimensions
