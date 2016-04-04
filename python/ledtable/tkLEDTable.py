#!/usr/bin/python3

try:
    from tkinter import Frame, Tk, Toplevel, Canvas
except ImportError:
    from Tkinter import Frame, Tk, Toplevel, Canvas
from itertools import chain

class TkLEDTableMixin(object):

    BACKGROUND_COLOR = "black"
    BORDER_COLOR = "gray"
    PIXEL_ORDER_COLOR = "white"
    PIXEL_WIDTH = 30
    PIXEL_HEIGHT = 30
    PIXEL_BORDER = 1
    
    def __init__(self, *args, **kw):
        self._canvas = Canvas(self, width = 0, height = 0)
        self._canvas.pack()
        self._pixels = []
        self._saved_pixels = []
        self._pixel_order = []
        self._pixel_width = self.PIXEL_WIDTH
        self._pixel_height = self.PIXEL_HEIGHT
        self._pixel_border = self.PIXEL_BORDER
        self._height = self._width = 0

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
        for y in range(self.height):
            pixels = []
            self._pixels.append(pixels)
            for x in range(self.width):
                pixel = self._canvas.create_rectangle(0,0,1,1)
                pixels.append(pixel)
        self._update_configuration()
        for y, row in enumerate(self._saved_pixels[:self.height]):
            for x, color in enumerate(row[:self.width]):
                self.set_pixel_color(x, y, color)

    def _remove_all_pixels(self):
        self._saved_pixels = [[self.get_pixel_color(x, y) for x in range(self.width)] for y in range(self.height)]
        for pixel in self._all_pixels:
            self._canvas.delete(pixel)
        self._pixels = []
        self._remove_pixel_order()

    def _remove_pixel_order(self):
        for line in self._pixel_order:
            self._canvas.delete(line)
        self._pixel_order = []

    @property
    def _all_pixels(self):
        return chain.from_iterable(self._pixels)

    def _update_configuration(self):
        for y, row in enumerate(self._pixels):
            for x, pixel in enumerate(row):
                x_cor = x * self.pixel_width
                y_cor = y * self.pixel_height
                self._canvas.coords(pixel, x_cor, y_cor,
                                           x_cor + self.pixel_width,
                                           y_cor + self.pixel_height)
                self._canvas.itemconfigure(pixel, outline = self.BORDER_COLOR,
                                                  fill = self.BACKGROUND_COLOR,
                                                  width = self.pixel_border)
        self._canvas.configure(width = self.width * self.pixel_width,
                               height = self.height * self.pixel_height)
    # pixel configuration

    @property
    def pixel_width(self):
        return self._pixel_width

    @pixel_width.setter
    def pixel_width(self, value):
        self._pixel_width = value
        self._update_configuration()
        self._remove_pixel_order()

    @property
    def pixel_height(self):
        return self._pixel_height

    @pixel_height.setter
    def pixel_height(self, value):
        self._pixel_height = value
        self._update_configuration()
        self._remove_pixel_order()

    @property
    def pixel_border(self):
        return self._pixel_border

    @pixel_border.setter
    def pixel_border(self, value):
        self._pixel_border = value
        self._update_configuration()
        
    # basic interface
    
    def set_pixel_color(self, x, y, color):
        self._canvas.itemconfigure(self._pixels[y][x], fill = color)

    def get_pixel_color(self, x, y):
        return self._canvas.itemcget(self._pixels[y][x], "fill")

    def show_pixel_order(self, coordinates):
        x_offset = (self.pixel_width + 1) / 2
        y_offset = (self.pixel_height + 1) / 2
        cor = [(x * self.pixel_width + x_offset, y * self.pixel_height + y_offset)
               for x, y in coordinates]
        for p1, p2 in zip(cor[:-1], cor[1:]):
            (x1, y1), (x2, y2) = p1, p2
            self._pixel_order.append(self._canvas.create_line(x1, y1, x2, y2, fill = self.PIXEL_ORDER_COLOR))

class LEDFrame(TkLEDTableMixin, Frame):
    def __init__(self, *args, **kw):
        Frame.__init__(self, *args, **kw)
        TkLEDTableMixin.__init__(self)

class LEDTk(TkLEDTableMixin, Tk):
    def __init__(self, *args, **kw):
        Tk.__init__(self, *args, **kw)
        TkLEDTableMixin.__init__(self)

class LEDToplevel(TkLEDTableMixin, Toplevel):
    def __init__(self, *args, **kw):
        Toplevel.__init__(self, *args, **kw)
        TkLEDTableMixin.__init__(self)

if __name__ == "__main__":
    l = LEDTk()
    l.dimensions = 12, 12
    l.set_pixel_color(5, 5, "blue")
