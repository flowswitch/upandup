#!/usr/bin/python3

from time import sleep

from up.up1024p import *
from up.pinmap import DIP48


def FindPin(data):
    res = None
    for i in range(32):
        if not data[i]:
            continue
        for bit in range(8):
            if (data[i]>>bit) & 1:
                if res==None:
                    res = i*8+bit
                else:
                    print(res, "short to", i*8+bit)
    return res


up = UP1024P()

print("Loading firmware...")
up.load_fx2('../FX2/build/gpio256.ihx', 'ihex')
print("OK")

up.get_status()
up.disable_pin_drivers()

print("Loading FPGA 0...")
up.load_fpga(0, 'fpga0.bin')
print("OK")

print("Loading FPGA 1...")
up.load_fpga(1, 'fpga1.bin')
print("OK")

up.get_status()

up.leds_off()
up.beep()

up.set_voltages(vcc=3.3, vio=3.3, vpp=12)
up.get_status()

up.reset_pin_drivers()

up.gpio_oe(b'\x00'*32)

while True:
    fpga_pin = FindPin(up.gpio_in())
    print("\r", fpga_pin, "                   ", end='')

