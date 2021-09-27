//#include "fx2.h"
#include "fx2regs.h"
#include "fx2sdly.h"            // SYNCDELAY macro

extern void GpifInit(void);

void UpInit(void)
{
        EP2CFG = 0xA0;
        SYNCDELAY;
        EP4CFG = 0;
        SYNCDELAY;
        EP6CFG = 0xE0;
        SYNCDELAY;
        EP8CFG = 0;
        SYNCDELAY;
        EP8CFG = 0;
        FIFORESET = 0x80;
        SYNCDELAY;
        FIFORESET = 2;
        SYNCDELAY;
        FIFORESET = 6;
        SYNCDELAY;
        FIFORESET = 0;
        SYNCDELAY;
        EP2BCL = 0x80;
        SYNCDELAY;
        EP2BCL = 0x80;
        SYNCDELAY;
        EP2BCL = 0x80;
        SYNCDELAY;
        EP2BCL = 0x80;
        SYNCDELAY;
        SYNCDELAY;
        EP2FIFOCFG = 0;
        SYNCDELAY;
        EP6FIFOCFG = 0;
        SYNCDELAY;
        EP4FIFOCFG = 0;
        SYNCDELAY;
        EP8FIFOCFG = 0;
        SYNCDELAY;
        EP1OUTCFG = 0xA0;
        SYNCDELAY;
        EP1INCFG = 0xA0;
        SYNCDELAY;
        EP1OUTBC = 0x80;
        SYNCDELAY;
        GpifInit();
        CPUCS = 0x10;
        OED = 0xFF;
        IOD = 0xFF;
        OEA = 0xFF;
        IOA = 0xFF;
        TMOD = 1;
        TR0 = 1;
}
