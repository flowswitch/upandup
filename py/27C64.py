#!/usr/bin/python3

from sys import argv, exit

if len(argv)!=2:
    exit("Usage: "+argv[0]+" <outfile>")

from struct import pack

from up.up1024p import *
from up.pinmap import DIP48

def set_bit(buf, bit):
    buf[bit>>3] |= 1<<(bit & 7)

def clr_bit(buf, bit):
    buf[bit>>3] &= ~(1<<(bit & 7))

def get_bit(buf, bit):
    return (buf[bit>>3]>>(bit & 7)) & 1

class S27C64:
    def __init__(self):
        self.up = UP1024P()

        print("Loading firmware...")
        self.up.load_fx2('../FX2/build/gpio256.ihx', 'ihex')
        print("OK")

        self.up.get_status()
        self.up.disable_pin_drivers()

        print("Loading FPGA 0...")
        self.up.load_fpga(0, 'fpga0.bin')
        print("OK")

        print("Loading FPGA 1...")
        self.up.load_fpga(1, 'fpga1.bin')
        print("OK")

        self.up.get_status()

        self.up.leds_off()
        self.up.beep()

        self.up.set_voltages(vcc=5.8, vio=3.3, vpp=12)
        self.up.get_status()

        self.up.reset_pin_drivers()

        pm = DIP48(28)

        self.up.set_pin_drive(pm.get(1), PIN_DRV.VCC)    # Vpp
        self.up.set_pin_drive(pm.get(28), PIN_DRV.VCC)   # Vcc
        self.up.set_pin_drive(pm.get(14), PIN_DRV.GND)   # Vss

        a_pins = (10, 9, 8, 7, 6, 5, 4, 3, 25, 24, 21, 23, 2)
        self.A = []
        for pin in a_pins:
            self.A.append(pm.get(pin))
        self.MAXADDR = len(a_pins)

        d_pins = (11, 12, 13, 15, 16, 17, 18, 19)    
        self.D = []
        for pin in d_pins:
            self.D.append(pm.get(pin))

        self.nCE = pm.get(20)
        self.nOE = pm.get(22)
        self.nPGM = pm.get(27)

        self.pcfg_in = bytearray(32)
        set_bit(self.pcfg_in, self.nPGM)

        self.idle = bytearray(32)
        self.idle[:] = self.pcfg_in
        #print("IDLE:", self.idle.hex())

        set_bit(self.pcfg_in, self.nCE)
        set_bit(self.pcfg_in, self.nOE)

        for i in range(self.MAXADDR):
            set_bit(self.pcfg_in, self.A[i])
        #print("INOE:", self.pcfg_in.hex())

        input("Insert the IC then press Enter...")    
        self.up.gpio_out(self.idle)
        self.up.gpio_oe(self.pcfg_in)
        self.up.apply_pin_drivers()

        # self.pcfg_out = bytearray(32)
        # self.pcfg_out[:] = self.pcfg_in
        # for i in range(8):
        #     set_bit(self.pcfg_out, self.D[i])


    def read(self, a):
        buf = bytearray(32)
        buf[:] = self.idle
        for bit in range(self.MAXADDR):
            if a & (1<<bit):
                set_bit(buf, self.A[bit])
        #input("Press Enter to set A...")
        # self.up.gpio_out(buf)
        # clr_bit(buf, self.nCE)
        #input("Press Enter to assert nCE...")
        # self.up.gpio_out(buf)
        # clr_bit(buf, self.nOE)
        #input("Press Enter to assert nOE...")
        self.up.gpio_out(buf)

        #input("Press Enter to sample DATA...")
        buf = self.up.gpio_in()

        # self.up.gpio_out(self.pcfg_in)

        res = 0
        for bit in range(8):
            res >>= 1
            if get_bit(buf, self.D[bit]):
                res |= 0x80
        return res


ic = S27C64()
print("Reading...")
with open(argv[1], 'wb') as hfo:
    for addr in range(8192):
        hfo.write(pack('<B', ic.read(addr)))
