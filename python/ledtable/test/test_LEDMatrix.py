import pytest

from ledtable.LEDMatrix import LEDMatrix

class SimpleLEDTable(LEDMatrix):

    def __init__(self):
        pass
    
    def _height(self):
        return 20

    def _width(self):
        return 15

    def _fill_pixel(self, x, y, color):
        self.last_color = color

@pytest.fixture
def t():
    return SimpleLEDTable()

def test_height(t):
    assert t.height == 20

def test_width(t):
    assert t.width == 15

def test_dimensions(t):
    assert t.dimensions == (t.width, t.height)

def test_set_height(t):
    with pytest.raises(NotImplementedError):
        t.height = 10
    
def test_set_width(t):
    with pytest.raises(NotImplementedError):
        t.height = 10
    
def test_set_dimensions(t):
    with pytest.raises(NotImplementedError):
        t.dimensions = (10, 10)
        
def test_min_x(t):
    assert t.min_x == 0

def test_min_y(t):
    assert t.min_y == 0

def test_max_x(t):
    assert t.max_x == 14

def test_max_y(t):
    assert t.max_y == 19

def test_middle_x(t):
    assert t.middle_x == 7

def test_middle_y(t):
    assert t.middle_y == 10

def test_brightness(t):
    assert t.brightness == 1

def test_set_brightness(t):
    t.brightness = 0.5
    assert t.brightness == 0.5
    with pytest.raises(ValueError):
        t.brightness = -0.1
    with pytest.raises(ValueError):
        t.brightness = 2
    assert t.brightness == 0.5
    t.fill_pixel(1, 1, (0,0,2))
    assert t.last_color == (0,0,1)

def test_
