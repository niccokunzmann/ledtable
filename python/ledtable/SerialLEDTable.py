#!/usr/bin/python3

from . tkLEDTable import *
try:
    import serial
except:
    import sys
    print("Install the serial module width '{} -m pip install PySerial'.".format(sys.executable))
    raise
import threading
try:
    from queue import Queue
except ImportError:
    from Queue import Queue
import itertools

def list_serial_ports():
    ## got the code from
    ## http://stackoverflow.com/questions/12090503/listing-available-com-ports-with-python
    import os
    from serial.tools import list_ports
    # Windows
    if os.name == 'nt':
        # Scan for available ports.
        available = []
        for i in range(256):
            try:
                s = serial.Serial(i)
                available.append('COM'+str(i + 1))
                s.close()
            except serial.SerialException:
                pass
        return available
    else:
        # Mac / Linux
        return [port[0] for port in list_ports.comports()]

class SerialLEDTable(threading.Thread):
    
    COMMAND_CHARACTER = b"!"
    
    def __init__(self, led_table, file):
        super(SerialLEDTable, self).__init__()
        self.led_table = led_table
        self.file = file

    def run(self):
        for line in self.file:
            if not line.startswith(self.COMMAND_CHARACTER):
                self.default_line(line)
            else:
                self.command_line(line[1:])

    def default_line(self, line):
        if line.endswith(b"\n"):
            line = line[:-1]
        if line.endswith(b"\r"):
            line = line[:-1]
        try:
            line = line.decode("ASCII")
        except UnicodeDecodeError:
            pass
        print(line)

    def command_line(self, line):
        command = line.split()
        if command[0] == b"p":
            height = int(command[1])
            self.set_pixels(height)
        elif command[0] == b"o":
            height = int(command[1])
            self.set_pixel_order(height)
        else:
            print("Invalid Command: {}".format(command))

    def set_pixel_order(self, height):
        indices = [[(int(i), x, y) for x, i in enumerate(self.file.readline().rstrip().split())]
                   for y in range(height)]
        indices = list(itertools.chain.from_iterable(indices))
        indices.sort()
        coordinates = [(x, y) for i, x, y in indices]
        self.set_pixel_order_on_ledtable(coordinates)

    def set_pixel_order_on_ledtable(self, coordinates):
        self.led_table.show_pixel_order(coordinates)

    def set_pixels(self, height):
        for y in range(height):
            line = self.file.readline().rstrip()
            pixels = line.decode("ASCII").split("#")[1:]
            for x, pixel in enumerate(pixels):
                self.set_pixel(x, y, pixel)

    def set_pixel(self, x, y, color):
        if not color:
            return # nothing happened
        if len(color) > 6:
            color = color[-6:]
        else:
            color = color.rjust(6, "0")
        color = "#" + color
        self.set_pixel_on_ledtable(x, y, color)

    def set_pixel_on_ledtable(self, x, y, color):
        if x >= self.led_table.width:
            self.led_table.width = x + 1
        if y >= self.led_table.height:
            self.led_table.height = y + 1
        self.led_table.set_pixel_color(x, y, color)


class TkSerialLEDTable(SerialLEDTable):

    def __init__(self, *args, **kw):
        super(TkSerialLEDTable, self).__init__(*args, **kw)
        self.queue = Queue()
        self.led_table.after(1, self.loop)

    def set_pixel_on_ledtable(self, *args):
        self.queue.put((super(TkSerialLEDTable, self).set_pixel_on_ledtable, args, {}))
    def set_pixel_order_on_ledtable(self, *args):
        self.queue.put((super(TkSerialLEDTable, self).set_pixel_order_on_ledtable, args, {}))

    def loop(self):
        while not self.queue.empty():
            func, args, kw = self.queue.get(block = False)
            func(*args, **kw)
        self.led_table.after(1, self.loop)

if __name__ == "__main__":
    import sys
    if not sys.argv[1:]:
        ports = list_serial_ports()
        if not ports:
            print("No serial ports detected. You should pass one as argument.")
        else:
            print('The first argument should be a serial port, e.g. "{}"'.format(
                     "\", \"".join(map(str, ports))))
        print("The second argument can be the baud rate of the serial connection.")
        print("If there is no second argument we assume 9600.")
        exit(1)
    port = sys.argv[1]
    if len(sys.argv) >= 3:
        baudrate = sys.argv[2]
    else:
        baudrate = 9600
    s = serial.serial_for_url(port, baudrate = baudrate)
    t = LEDTk()
    t.title(port)
    ts = TkSerialLEDTable(t, s)
    ts.deamon = True
    ts.start()
    try:
        t.mainloop()
    finally:
        s.close()
