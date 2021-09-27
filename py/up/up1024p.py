'''UP-1024P low level access'''

__all__ = ['UP1024P', 'UPError', 'PIN_DRV']

from enum import IntEnum
from struct import pack, unpack
from fx2 import VID_CYPRESS, PID_FX2, FX2Config, FX2Device, FX2DeviceError
from fx2.format import input_data


VID_UP = VID_CYPRESS
PID_UP = 0x1234
CMD_EP = 1
STS_EP = 0x81
DATA_OUT_EP = 2
DATA_IN_EP = 0x86
FPGA_SIZE = 0x1110C


class UPError(Exception):
    pass


class CMD(IntEnum):
    LOAD_FPGA0 = 0xD0
    LOAD_FPGA1 = 0xCF
    PING =       0xD1
    PINS_OE =    0xD2
    PINS_WR =    0xD3
    PINS_RD =    0xD4
    DRV_OFF =    0xD5
    DRV_ZERO =   0xD6
    DRV_SET =    0xD7
    DRV_LOAD =   0xD8
    VCC_RAMP =   0xDB
    SET_VREGS =  0xDC
    LEDS_OFF =   0xDD
    LED_G =      0xDE
    LED_R =      0xDF
    BEEP =       0xE0
    CPLD_WR =    0xE8
    CPLD_RD =    0xE9


class PIN_DRV(IntEnum):
    GND = 0
    VCC = 1
    VPP = 2


class UP1024P:
    def __init__(self, vid=VID_UP, pid=PID_UP):
        self.dev = FX2Device(vid, pid)
        self.dev.usb.claimInterface(0)
        self.dev.usb.setInterfaceAltSetting(0, 1)


    def load_fx2(self, fwfile, fmt='auto'):
        with open(fwfile, 'rb') as hfi:
            self.dev.load_ram(input_data(hfi, fmt))
        self.dev.usb.setInterfaceAltSetting(0, 1)

    ############## UP fw commands ##################        

    def send_cmd(self, cmd):
        self.dev.bulk_write(CMD_EP, pack('<B', cmd))


    def get_status(self):
        return unpack('<H', self.dev.bulk_read(STS_EP, 2))[0]


    def ping(self):
        self.send_cmd(CMD.PING)
        sts = self.get_status()
        if sts!=0x0101:
            raise UPError("Bad UP state: %04X" % (sts))


    def load_fpga(self, chip, fwfile):
        data = open(fwfile, 'rb').read()
        if len(data)!=FPGA_SIZE:
            raise ValueError("Invalid FPGA image size")
        self.send_cmd({ 0: CMD.LOAD_FPGA0, 1: CMD.LOAD_FPGA1 }[chip])
        self.dev.bulk_write(DATA_OUT_EP, data)
        sts = self.get_status()
        if sts!=0x0101:
            raise UPError("FPGA load error: %04X" % (sts))


    def leds_off(self):
        self.send_cmd(CMD.LEDS_OFF)


    def led_g(self):
        self.send_cmd(CMD.LED_G)


    def led_r(self):
        self.send_cmd(CMD.LED_R)


    def beep(self):
        self.send_cmd(CMD.BEEP)


    def cpld_write(self, addr, val):
        self.dev.bulk_write(CMD_EP, pack('<3B', CMD.CPLD_WR, addr, val))            


    def cpld_read(self, addr):
        self.dev.bulk_write(CMD_EP, pack('<2B', CMD.CPLD_RD, addr))            
        return self.dev.bulk_read(STS_EP, 1)[0]
      
    ############ pin driver config ################

    def set_voltages(self, vcc, vio, vpp, vmask=0):
        if vcc>0:
            vcc_pwm = round(vcc*39-49)
            if vcc_pwm<1 or vcc_pwm>255:
                raise ValueError("Vcc is out of range")
        else:
            vcc_pwm = 1

        if vio>0:
            if vio>3.3:
                raise ValueError("Vio is out of range")
            vio_pwm = round(vio*65-1.25)
            if vio_pwm<1 or vio_pwm>255:
                raise ValueError("Vio is out of range")
        else:
            vio_pwm = 1
            
        if vpp>0:
            vpp_pwm = round(vpp*16.4-31.88)
            if vpp_pwm<0xA0 or vio_pwm>255:
                raise ValueError("Vpp is out of range")
        else:
            vpp_pwm = 1

        self.dev.bulk_write(CMD_EP, pack('<5B', CMD.SET_VREGS, vcc_pwm, vio_pwm, vpp_pwm, vmask))            


    def disable_pin_drivers(self):
        self.send_cmd(CMD.DRV_OFF)


    def reset_pin_drivers(self):
        self.send_cmd(CMD.DRV_ZERO)


    def set_pin_drive(self, pin, typ):
        if pin<0 or pin>255:
            raise ValueError("Invalid pin index")

        if not isinstance(typ, PIN_DRV):
            raise TypeError("Invalid drive type")

        self.dev.bulk_write(CMD_EP, pack('<3B', CMD.DRV_SET, pin, typ))            


    def apply_pin_drivers(self):
        self.send_cmd(CMD.DRV_LOAD)

    ######### GPIO256-specific commands. Move to GPIO256 ? ##################

    def gpio_oe(self, oe):
        self.dev.bulk_write(CMD_EP, CMD.PINS_OE+oe)            


    def gpio_out(self, out):
        self.dev.bulk_write(CMD_EP, CMD.PINS_WR+out)            


    def gpio_in(self):
        self.send_cmd(CMD.PINS_RD)
        return self.dev.bulk_read(STS_EP, 32)

    ######### misc ###############

    def ramp_vcc(self):
        self.send_cmd(CMD.VCC_RAMP)
        return self.dev.bulk_read(STS_EP, 1)[0]
      
        
        