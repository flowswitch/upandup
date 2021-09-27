#ifndef _FPGA_H_INCLUDED
#define _FPGA_H_INCLUDED

extern BYTE LoadFpga(BYTE chip);
extern void FpgaAddr(BYTE addr);
extern void FpgaWrite(BYTE val);
extern BYTE FpgaRead();

#endif
