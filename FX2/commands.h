#ifndef _COMMANDS_H_INCLUDED
#define _COMMANDS_H_INCLUDED

enum
{
    CMD_CPLD_WR		= 0xC0,
    CMD_CPLD_RD     = 0xC1,
    CMD_SET_VREGS   = 0xC2,
    CMD_VCC_RAMP    = 0xC3,
    CMD_GET_STATUS  = 0xC4,
    CMD_LEDS_OFF    = 0xC5,
    CMD_LED_G       = 0xC6,
    CMD_LED_R       = 0xC7,
    CMD_BEEP        = 0xC8,
    CMD_GET_SN      = 0xC9,

    CMD_DRV_OFF     = 0xD0,
    CMD_DRV_ZERO    = 0xD1,
    CMD_DRV_SET     = 0xD2,
    CMD_DRV_LOAD    = 0xD3,

    CMD_PINS_OE     = 0xE0,
    CMD_PINS_WR     = 0xE1,
    CMD_PINS_RD     = 0xE2,

    CMD_LOAD_FPGA0  = 0xF0,
    CMD_LOAD_FPGA1  = 0xF1,
    CMD_FPGA_WR     = 0xF2,
    CMD_FPGA_RD     = 0xF3,
} Cmd_t;

#endif
