import time
import board
import busio

from digitalio import Direction, Pull, DigitalInOut

from adafruit_mcp230xx.mcp23017 import MCP23017

# Initialize the I2C bus:
i2c = busio.I2C(board.SCL, board.SDA)

# Initialize the MCP23017 chip on the bonnet
mcp = MCP23017(i2c, address=0x20)

print("MCP23017 ok")

port_a_pins = []
for pin in range(0, 8):
    port_a_pins.append(mcp.get_pin(pin))
# Set all the port A pins to input, with pullups!
for pin in port_a_pins:
    pin.direction = Direction.INPUT
    pin.pull = Pull.UP

print("BUTTONS ok")

port_b_pins = []
for pin in range(8, 12):
    port_b_pins.append(mcp.get_pin(pin))
# Set all the port B pins to input, with pullups!
for pin in port_b_pins:
    pin.direction = Direction.INPUT
    pin.pull = Pull.UP

print("CODEC ok")

#LEDS!!!
led_pins = [DigitalInOut(board.D12),
            DigitalInOut(board.D13),
            DigitalInOut(board.A0),
            DigitalInOut(board.A1),
            DigitalInOut(board.A2),
            DigitalInOut(board.A3),
            DigitalInOut(board.A4),
            DigitalInOut(board.A5)]

# set LEDs as output
for pin in led_pins:
    pin.direction = Direction.OUTPUT

print("LEDS ok")

print("PRESS ANY BUTTON")

while True: #for i in range(1,1000):
    cb= [not p.value for p in port_b_pins]
    
    codec = cb[0] + cb[1]*2 + cb[2]*4 + cb[3]*8
    
    for num, button in enumerate(port_a_pins):
        if not button.value:
            print("Button ", num+1, " CD = ", codec)
            # turn on matching led pin
            led_pins[num].value = True  # turn LED on!
        else:
            led_pins[num].value = False  # turn LED off
    time.sleep(0.1)

