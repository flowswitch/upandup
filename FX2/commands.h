#ifndef _COMMANDS_H_INCLUDED
#define _COMMANDS_H_INCLUDED

#define CMD_LOAD_FPGA0  0xD0
#define CMD_LOAD_FPGA1  0xCF
#define CMD_PING        0xD1
#define CMD_PINS_OE     0xD2
#define CMD_PINS_WR     0xD3
#define CMD_PINS_RD     0xD4
#define CMD_DRV_OFF     0xD5
#define CMD_DRV_ZERO    0xD6
#define CMD_DRV_SET     0xD7
#define CMD_DRV_LOAD    0xD8
#define CMD_VCC_RAMP	0xDB
#define CMD_SET_VREGS   0xDC
#define CMD_LEDS_OFF    0xDD
#define CMD_LED_G       0xDE
#define CMD_LED_R       0xDF
#define CMD_BEEP        0xE0

#define CMD_CPLD_WR		0xE8
#define CMD_CPLD_RD		0xE9

#endif
