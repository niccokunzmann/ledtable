#!/usr/bin/python3

from tkinter import Frame, Tk, Toplevel
from itertools import chain

class TkLEDTableMixin:

    BACKGROUND_COLOR = "black"
    PIXEL_WIDTH = 30
    PIXEL_HEIGHT = 30
    PIXEL_BORDER = 1
    
    def __init__(self, *args, **kw):
        super().__init__(*args, **kw)
        self._pixels = []
        self.dimensions = 0, 0
        self.pixel_width = self.PIXEL_WIDTH
        self.pixel_height = self.PIXEL_HEIGHT
        self.pixel_border = self.PIXEL_BORDER

    # initial configuration

    @property
    def width(self):
        return self._width

    @width.setter
    def width(self, value):
        self.dimensions = value, self.height
        
    @property
    def height(self):
        return self._height

    @height.setter
    def height(self, value):
        self.dimensions = self.width, value

    @property
    def dimensions(self):
        return self._width, self._height

    @dimensions.setter
    def dimensions(self, value):
        width, height = value
        self._remove_all_pixels()
        self._width, self._height = width, height
        self._create_pixels()

    # utility methods

    def _create_pixels(self):
        assert not self._pixels
        for x in range(self.width):
            pixels = []
            self._pixels.append(pixels)
            for y in range(self.height):
                pixel = Frame(self, background = self.BACKGROUND_COLOR)
                pixel.grid(column = x, row = y)
                pixels.append(pixel)
        self._update_configuration()

    def _remove_all_pixels(self):
        for pixel in self._all_pixels:
            pixel.grid_forget()
        self._pixels = []

    @property
    def _all_pixels(self):
        return chain.from_iterable(self._pixels)

    def _update_configuration(self):
        for pixel in self._all_pixels:
            pixel.configure(width = self.pixel_width,
                            height = self.pixel_height,
                            border = self.pixel_border)

    # pixel configuration

    @property
    def pixel_width(self):
        return self._pixel_width

    @pixel_width.setter
    def pixel_width(self, value):
        self._pixel_width = value
        self._update_configuration()

    @property
    def pixel_height(self):
        return self._pixel_height

    @pixel_height.setter
    def pixel_height(self, value):
        self._pixel_height = value
        self._update_configuration()

    @property
    def pixel_border(self):
        return self._pixel_border

    @pixel_border.setter
    def pixel_border(self, value):
        self._pixel_border = value
        self._update_configuration()
        
    # basic interface
    
    def set_pixel_color(self, x, y, color):
        self._pixels[x][y]["bg"] = color

    def get_pixel_color(self, x, y):
        return self._pixels[x][y]["bg"]

class LEDFrame(TkLEDTableMixin, Frame):
    pass

class LEDTk(TkLEDTableMixin, Tk):
    pass

class LEDToplevel(TkLEDTableMixin, Toplevel):
    pass

if __name__ == "__main__":
    l = LEDTk()
    l.dimensions = 12, 12
    l.set_pixel_color(5, 5, "blue")
