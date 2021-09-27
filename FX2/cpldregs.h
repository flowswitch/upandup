#ifndef _CPLDREGS_H_INCLUDED
#define _CPLDREGS_H_INCLUDED

#define CPLD_REG_VPP_L      0x00
#define CPLD_REG_VPP_H      0x01

#define CPLD_REG_DRVCTL     0x02
#define CPLD_DRVCTL_STB     1
#define CPLD_DRVCTL_OE2367  2
#define CPLD_DRVCTL_OE0145  4

#define CPLD_REG_DRV01DAT   0x03
#define CPLD_DRVDAT_D0  1
#define CPLD_DRVDAT_D1  2
#define CPLD_DRVDAT_D2  4
#define CPLD_DRVDAT_D3  8
#define CPLD_DRVDAT_CLK 0x10

#define CPLD_REG_VCC_L      0x10
#define CPLD_REG_VCC_H      0x11

#define CPLD_REG_VIO_L      0x12
#define CPLD_REG_VIO_H      0x13

#define CPLD_REG_GPO        0x14
#define CPLD_GPO_LED_R   2
#define CPLD_GPO_LED_G   1

#define CPLD_REG_DCDCRANGE	0x15
#define CPLD_DCDCRANGE_VCC  2
#define CPLD_DCDCRANGE_VPP  4

#define CPLD_REG_PUPD       0x16
#define CPLD_PUPD_PD    1
#define CPLD_PUPD_PU    2

#define CPLD_REG_BUZZER     0x17
#define CPLD_BUZZER 1

#define CPLD_REG_VCCEN		0x18
#define CPLD_VCCEN  1

#define CPLD_REG_DRV23DAT   0x19
//same as DRV01DAT

#define CPLD_REG_PUPD2      0x1A
//write same value as to PUPD

#endif
