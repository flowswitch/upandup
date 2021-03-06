#include "fx2.h"
#include "fx2regs.h"
#include "fx2sdly.h"            // SYNCDELAY macro
#include "cpld.h"
#include "ports.h"

#define SPARTAN_CFG_SIZE 0x1110C

BYTE LoadFpga(BYTE chip)
{
    BYTE uc;
    DWORD ul;
    WORD len;

    SetVio(VIO_3V3);
    mdelay(20);
    OEA = 0x5C;
    //enter configuration mode
    if (chip==0)
    {   //loading FPGA0 triggers config reset of both FPGAs
        //pulse config reset
        IO_FPGA_nPROGRAM = 0;
        for(uc=0; uc<250; uc++);
        IO_FPGA_nPROGRAM = 1;
        TH0 = 0;
        while( (TH0<200) && (!IO_FPGA_nINIT) );
        //select FPGA0
        IO_FPGA_nWRITE = 0;
        IO_FPGA_nCS0 = 0;
        IO_FPGA_nCS1 = 1;
    }
    else
    {   //no reset, just select FPGA1
        IO_FPGA_nWRITE = 0;
        IO_FPGA_nCS0 = 1;
        IO_FPGA_nCS1 = 0;
    }
    //load config from USB via GPIF
    for(ul=0; ul<SPARTAN_CFG_SIZE;)
    {
        while(EP2468STAT & bmEP2EMPTY);
        len = (EP2BCH<<8) | EP2BCL;
        if (len!=0)
        {
            EP2GPIFTCH = EP2BCH;
            SYNCDELAY;
            EP2GPIFTCL = EP2BCL;
            SYNCDELAY;
            EP2BCL = 0;
            SYNCDELAY;
            GPIFTRIG = 0;
            SYNCDELAY;
            while((GPIFTRIG & 0x80)==0);
        }
        ul+=len;
    }
    //get DONE status
    if (chip==0)
        uc = IO_FPGA_DONE0 ? 1 : 0;
    else
        uc = IO_FPGA_DONE1 ? 1 : 0;
    SendStatus(uc);
    //deselect
    if (chip==0)
    {
        OEA = 0x7C;
        IO_FPGA_nCS0 = 1;
        IO_FPGA_nCS0 = 0;
    }
    else
    {
        OEA = 0x6C;
        IO_FPGA_nCS1 = 1;
        IO_FPGA_nCS1 = 0;
    }
    return uc;
}

void FpgaAddr(BYTE addr)
{
    IO_FPGA_nINIT = 1;
    //  SYNCDELAY;
    while((GPIFTRIG & 0x80)==0);
    //  SYNCDELAY;
    XGPIFSGLDATLX = addr;
    //  SYNCDELAY;
    //  while((GPIFTRIG & 0x80)==0);
    //  SYNCDELAY;
    IO_FPGA_nINIT = 0;
    //  SYNCDELAY;
}

void FpgaWrite(BYTE val)
{
    while((GPIFTRIG & 0x80)==0);
    SYNCDELAY;
    XGPIFSGLDATLX = val;
    //  SYNCDELAY;
    //  while((GPIFTRIG & 0x80)==0);
    //  SYNCDELAY;
}

BYTE FpgaRead()
{
    BYTE g_data;

    while((GPIFTRIG & 0x80)==0);  // poll GPIFTRIG.7 Done bit
    SYNCDELAY;
    g_data = XGPIFSGLDATLX;       // trigger GPIF read
    while((GPIFTRIG & 0x80)==0); // poll GPIFTRIG.7 Done bit
    SYNCDELAY;
    g_data = XGPIFSGLDATLNOX;     // get read result, do not trigger
    //  SYNCDELAY;
    return g_data;
}

void FpgaWriteReg(BYTE addr, BYTE val)
{
    FpgaAddr(addr);
    FpgaWrite(val);
}

BYTE FpgaReadReg(BYTE addr)
{
    FpgaAddr(addr);
    return FpgaRead();
}
