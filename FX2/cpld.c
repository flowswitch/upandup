#include "string.h"
//#include "fx2.h"
#include "fx2regs.h"
#include "fx2sdly.h"            // SYNCDELAY macro
#include "cpldregs.h"
#include "ports.h"

__xdata static unsigned char DrvCfg[8][12];
static BYTE outport;

void mdelay(BYTE dly)
{
        BYTE u;

        for(u=0; u<dly; u++)
        {
                TL0 = 0x60;
                TH0 = 0;
                while(TH0<0x10);
        }
}

void CpldAddr(unsigned char addr)
{
        IOD = addr | 0xE0;
        IOD = addr | 0xC0;
        IOD = addr | 0xE0;
}

void CpldWrite(unsigned char val)
{
        IOD = val | 0x60;
        IOD = val | 0x40;
        IOD = val | 0x60;
}

unsigned char CpldRead(void)
{
        unsigned char val;
        OED = 0xE0;
        IOD = 0x20;
        val = IOD & 0x1F;
        IOD = 0x60;
        OED = 0xFF;
        return val;
}

void CpldWriteReg(unsigned char addr, unsigned char val)
{
        if (addr==0x16)
        {
                CpldAddr(0x1A);
                CpldWrite(val);
        }
        CpldAddr(addr);
        CpldWrite(val);
}

unsigned char CpldReadReg(unsigned char addr)
{
        CpldAddr(addr);
        return CpldRead();
}

void LoadDrivers(void)
{
        register unsigned char u,v,cf0,cf1,cf2,cf3,outval;

        CpldAddr(2);
        IOD = 0x40; //deassert STB
        //drivers 1,2 rows 0,A
        CpldAddr(CPLD_DRV01DAT);
        for(u=0; u<12; u++)
        {
                cf0 = DrvCfg[0][11-u];
                cf1 = DrvCfg[1][11-u];
                cf2 = DrvCfg[2][11-u];
                cf3 = DrvCfg[3][11-u];
                for(v=0; v<8; v++)
                {
                        outval = 0x40;
                        if (cf0 & 0x80)
                                outval |= 1;
                        if (cf1 & 0x80)
                                outval |= 2;
                        if (cf2 & 0x80)
                                outval |= 4;
                        if (cf3 & 0x80)
                                outval |= 8;
                        IOD = outval; //CLK low
                        IOD = outval | 0x10; //CLK high
                        cf0<<=1;
                        cf1<<=1;
                        cf2<<=1;
                        cf3<<=1;
                }
        }
        //drivers 3,4 rows 0,A
        CpldAddr(CPLD_DRV23DAT);
        for(u=0; u<12; u++)
        {
                cf0 = DrvCfg[4][11-u];
                cf1 = DrvCfg[5][11-u];
                cf2 = DrvCfg[6][11-u];
                cf3 = DrvCfg[7][11-u];
                for(v=0; v<8; v++)
                {
                        outval = 0x40;
                        if (cf0 & 0x80)
                                outval |= 1;
                        if (cf1 & 0x80)
                                outval |= 2;
                        if (cf2 & 0x80)
                                outval |= 4;
                        if (cf3 & 0x80)
                                outval |= 8;
                        IOD = outval; //CLK low
                        IOD = outval | 0x10; //CLK high
                        cf0<<=1;
                        cf1<<=1;
                        cf2<<=1;
                        cf3<<=1;
                }
        }
        CpldAddr(2);
        IOD = 0x41; //assert STB
        IOD = 0x40; //deassert STB
        outval = IO_CPLD_ERR ? 6 : 0; //OE
        IOD = outval | 0x40;
        IOD = outval | 0x60;
}

void ClearDrivers(void)
{
        memset(DrvCfg, 0, 12*8);
        LoadDrivers();
        CpldWriteReg(0x16, 1);
        mdelay(2);
}

void SetPinDrive(unsigned char pin, unsigned char type)
{
	// pin.7:4 - row, pin.4:3
        DrvCfg[pin>>5][((pin>>3) & 3)+4*type] |= 1<<(pin & 7);
}

void ClearPindrives(void)
{
        memset(DrvCfg, 0, 12*8);
}

void SetVcc(unsigned char pwm)
{
        CpldWriteReg(CPLD_VCC_L, pwm & 0x0F);
        CpldWriteReg(CPLD_VCC_H, pwm>>4);
}

void SetVio(unsigned char pwm)
{
        CpldWriteReg(CPLD_VIO_L, pwm & 0x0F);
        CpldWriteReg(CPLD_VIO_H, pwm>>4);
}

void SetVpp(unsigned char pwm)
{
        CpldWriteReg(CPLD_VPP_L, pwm & 0x0F);
        CpldWriteReg(CPLD_VPP_H, pwm>>4);
}

void SetVoltages(unsigned char vcc, unsigned char vio, unsigned char vpp, unsigned char vmask)
{
        if (vcc>115)
                vmask |= 2;
        if (vpp>177)
                vmask |= 4;
        if (vcc!=1)
                CpldWriteReg(0x18, 1);
		CpldWriteReg(CPLD_VMASK, vmask);
        SetVcc(vcc);
        SetVio(vio);
        SetVpp(vpp);
        mdelay(10);
}

void LedsOff()
{
  outport = 0;
  CpldWriteReg(CPLD_GPO, 0);
}

void LedGreen()
{
  outport |= CPLD_GPO_LED_G;
  CpldWriteReg(CPLD_GPO, outport);
}

void LedRed()
{
  outport |= CPLD_GPO_LED_R;
  CpldWriteReg(CPLD_GPO, outport);
}

void Beep()
{
  BYTE i,j;

  CpldAddr(CPLD_BUZZER);
  for(i=0; i<200u; i++)
  {
    CpldWrite(1);
    for(j=0; j<150u; j++);
    CpldWrite(0);
    for(j=0; j<150u; j++);
  }
}

void SendStatus(unsigned char sts)
{
  while(EP01STAT & bmBIT2);
  EP1INBUF[0] = sts;
  SYNCDELAY;
  CpldAddr(CPLD_VCC_L);
  EP1INBUF[1] = IO_ERR;
  SYNCDELAY;
  EP1INBC = 2;
}
