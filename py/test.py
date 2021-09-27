#!/usr/bin/python3

from up.up1024p import *
from up.pinmap import DIP48

up = UP1024P()

print("Loading firmware...")
up.load_fx2('../FX2/build/gpio256.ihx', 'ihex')
print("OK")

up.get_status()
up.disable_pin_drivers()

print("CPLD SN:", up.get_sn())

print("Loading FPGA 0...")
up.load_fpga(0, 'fpga0.bin')
print("OK")

print("Loading FPGA 1...")
up.load_fpga(1, 'fpga1.bin')
print("OK")

up.get_status()

up.leds_off()
up.beep()

up.set_voltages(vcc=2.5, vio=3.3, vpp=12)
up.get_status()

up.reset_pin_drivers()

pm = DIP48(8)

#for p in range(24):
#    #up.set_pin_drive(pm.get(p+1), PIN_DRV.VCC)
#    up.set_pin_drive(pm.get(p+25), PIN_DRV.GND)
up.set_pin_drive(pm.get(1), PIN_DRV.VCC)
#up.set_pin_drive(201, PIN_DRV.VCC)
up.set_pin_drive(pm.get(8), PIN_DRV.GND)
#up.set_pin_drive(pm.get(24), PIN_DRV.GND)
up.apply_pin_drivers()
