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
    CPLD_WR		= 0xC0
    CPLD_RD     = 0xC1
    SET_VREGS   = 0xC2
    VCC_RAMP    = 0xC3
    GET_STATUS  = 0xC4
    LEDS_OFF    = 0xC5
    LED_G       = 0xC6
    LED_R       = 0xC7
    BEEP        = 0xC8
    GET_SN      = 0xC9

    DRV_OFF     = 0xD0
    DRV_ZERO    = 0xD1
    DRV_SET     = 0xD2
    DRV_LOAD    = 0xD3

    PINS_OE     = 0xE0
    PINS_WR     = 0xE1
    PINS_RD     = 0xE2

    LOAD_FPGA0  = 0xF0
    LOAD_FPGA1  = 0xF1
    FPGA_WR     = 0xF2
    FPGA_RD     = 0xF3


class PIN_DRV(IntEnum):
    GND = 0
    VCC = 1
    VPP = 2
    OFF = 0xFF


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

    def send_cmd(self, cmd, data=b''):
        self.dev.bulk_write(CMD_EP, pack('<B', cmd)+data)


    def recv_status(self):
        return unpack('<H', self.dev.bulk_read(STS_EP, 2))[0]


    def get_status(self):
        self.send_cmd(CMD.GET_STATUS)
        sts = self.recv_status()
        if sts!=0x0101:
            raise UPError("Bad UP state: %04X" % (sts))


    def load_fpga(self, chip, fwfile):
        data = open(fwfile, 'rb').read()
        if len(data)!=FPGA_SIZE:
            raise ValueError("Invalid FPGA image size")
        self.send_cmd({ 0: CMD.LOAD_FPGA0, 1: CMD.LOAD_FPGA1 }[chip])
        self.dev.bulk_write(DATA_OUT_EP, data)
        sts = self.recv_status()
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
        self.send_cmd(CMD.CPLD_WR, pack('<2B', addr, val))            


    def cpld_read(self, addr):
        self.send_cmd(CMD.CPLD_RD, pack('<B', addr))            
        return self.dev.bulk_read(STS_EP, 1)[0]
      
    ############ pin driver config ################

    def set_voltages(self, *, vcc=3.3, vio=3.3, vpp=12, vmask=0):
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

        self.send_cmd(CMD.SET_VREGS, pack('<4B', vcc_pwm, vio_pwm, vpp_pwm, vmask))            


    def disable_pin_drivers(self):
        self.send_cmd(CMD.DRV_OFF)


    def reset_pin_drivers(self):
        self.send_cmd(CMD.DRV_ZERO)


    def set_pin_drive(self, pin, typ):
        if pin<0 or pin>255:
            raise ValueError("Invalid pin index")

        if not isinstance(typ, PIN_DRV):
            raise TypeError("Invalid drive type")

        self.send_cmd(CMD.DRV_SET, pack('<2B', pin, typ))            


    def apply_pin_drivers(self):
        self.send_cmd(CMD.DRV_LOAD)

    ######### GPIO256-specific commands. Move to GPIO256 ? ##################

    def gpio_oe(self, oe):
        self.send_cmd(CMD.PINS_OE, oe)            


    def gpio_out(self, out):
        self.send_cmd(CMD.PINS_WR, out)            


    def gpio_in(self):
        self.send_cmd(CMD.PINS_RD)
        return self.dev.bulk_read(STS_EP, 32)

    ######### misc ###############

    def ramp_vcc(self):
        self.send_cmd(CMD.VCC_RAMP)
        return self.dev.bulk_read(STS_EP, 1)[0]

    def get_sn(self):
        self.send_cmd(CMD.GET_SN)
        return unpack('<I', self.dev.bulk_read(STS_EP, 4))[0]
