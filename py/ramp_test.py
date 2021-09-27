#!/usr/bin/python3

from up.up1024p import *
from up.pinmap import DIP48

up = UP1024P()

print("Loading firmware...")
#up.load_fx2('up256.bin', 'bin')
up.load_fx2('../FX2/build/gpio256.ihx', 'ihex')
print("OK")

up.ping()
up.disable_pin_drivers()

# print("Loading FPGA 0...")
# up.load_fpga(0, 'fpga0.bin')
# print("OK")

# print("Loading FPGA 1...")
# up.load_fpga(1, 'fpga1.bin')
# print("OK")

# up.ping()

up.leds_off()
up.beep()

print(up.ramp_vcc())
