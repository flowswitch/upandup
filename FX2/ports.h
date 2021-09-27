#ifndef _PORTS_H_INCLUDED
#define _PORTS_H_INCLUDED

//Port A - FPGA config interface, CLPD ERR
__sbit __at 0x80+0 IO_CPLD_ERR;
__sbit __at 0x80+2 IO_FPGA_nPROGRAM;
__sbit __at 0x80+5 IO_FPGA_nINIT;
__sbit __at 0x80+4 IO_FPGA_nWRITE;
__sbit __at 0x80+3 IO_FPGA_nCS0;
__sbit __at 0x80+6 IO_FPGA_nCS1;
__sbit __at 0x80+1 IO_FPGA_DONE0;
__sbit __at 0x80+7 IO_FPGA_DONE1;


//Port D - CPLD bus
//CPLD regs are transparent latches. nWR=0 opens the latch, posedge(nWR) latches the last value
__sbit __at 0xB0+0 IO_CPLD_D0;
__sbit __at 0xB0+1 IO_CPLD_D1;
__sbit __at 0xB0+2 IO_CPLD_D2;
__sbit __at 0xB0+3 IO_CPLD_D3;
__sbit __at 0xB0+4 IO_CPLD_D4;
__sbit __at 0xB0+5 IO_CPLD_nWR;
__sbit __at 0xB0+6 IO_CPLD_nRD;
__sbit __at 0xB0+7 IO_CPLD_ADDR;

#define CPLD_ADDR_MASK  0x80
#define CPLD_nRD_MASK   0x40
#define CPLD_nWR_MASK   0x20

#endif
