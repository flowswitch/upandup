#define ALLOCATE_EXTERN
//#include "fx2.h"
#include "fx2regs.h"
#include "fx2sdly.h"            // SYNCDELAY macro

#include "cpld.h"
#include "fpga.h"
#include "commands.h"


void Command()
{
  BYTE cmd,p0,p1,p2,p3;
  cmd = EP1OUTBUF[0];
  switch(cmd)
  {
    case CMD_PING:
         EP1OUTBC = 0;
         SYNCDELAY;
         SendStatus(1);
         break;
    case CMD_LOAD_FPGA0:
         EP1OUTBC = 0;
         SYNCDELAY;
         LoadFpga(0);
         break;
    case CMD_LOAD_FPGA1:
         EP1OUTBC = 0;
         SYNCDELAY;
         LoadFpga(1);
         break;
    case CMD_PINS_OE:
         p0 = EP1OUTBC-1;
         for(p1=0; p1<p0; p1++)
         {
           FpgaAddr(0x80 | p1);
           FpgaWrite(EP1OUTBUF[p1+1]);
         }
         SYNCDELAY;
         EP1OUTBC = 0;
         break;
    case CMD_PINS_WR:
         p0 = EP1OUTBC-1;
         for(p1=0; p1<p0; p1++)
         {
           FpgaAddr(p1);
           FpgaWrite(EP1OUTBUF[p1+1]);
         }
         SYNCDELAY;
         EP1OUTBC = 0;
         break;
    case CMD_PINS_RD:
         EP1OUTBC = 0;
         SYNCDELAY;
         while(EP01STAT & bmBIT2);
         for(p1=0; p1<32; p1++)
         {
           FpgaAddr(p1);
           EP1INBUF[p1] = FpgaRead();
         }
         EP1INBC = 32;
         break;
    case CMD_DRV_OFF:
         EP1OUTBC = 0;
         SYNCDELAY;
         ClearDrivers();
         break;
    case CMD_DRV_ZERO:
         EP1OUTBC = 0;
         SYNCDELAY;
         ClearPindrives();
         break;
    case CMD_DRV_SET:
         p0 = EP1OUTBUF[1];
         p1 = EP1OUTBUF[2];
         SYNCDELAY;
         EP1OUTBC = 0;
         SYNCDELAY;
         SetPinDrive(p0, p1);
         break;
    case CMD_DRV_LOAD:
         EP1OUTBC = 0;
         SYNCDELAY;
         LoadDrivers();
         break;
	case CMD_VCC_RAMP:
         EP1OUTBC = 0;
         SYNCDELAY;
         ClearDrivers();
  		 SetVcc(20);
		 for(p0=20; p0<58; p0++)
		 {
			SetVcc(p0);
			mdelay(2);
			if (CpldReadReg(0x10) & 1)
				break;
		 }
         while(EP01STAT & bmBIT2);
         EP1INBUF[0] = p0;
         EP1INBC = 1;
		 SetVcc(0x4F);
		 break;
    case CMD_SET_VREGS:
         p0 = EP1OUTBUF[1];
         p1 = EP1OUTBUF[2];
         p2 = EP1OUTBUF[3];
         p3 = EP1OUTBUF[4];
         SYNCDELAY;
         EP1OUTBC = 0;
         SYNCDELAY;
         SetVoltages(p0, p1, p2, p3);
         break;
    case CMD_LEDS_OFF:
         EP1OUTBC = 0;
         SYNCDELAY;
         LedsOff();
         break;
    case CMD_LED_G:
         EP1OUTBC = 0;
         SYNCDELAY;
         LedGreen();
         break;
    case CMD_LED_R:
         EP1OUTBC = 0;
         SYNCDELAY;
         LedRed();
         break;
    case CMD_BEEP:
         EP1OUTBC = 0;
         SYNCDELAY;
         Beep();
         break;
	case CMD_CPLD_WR:
         p0 = EP1OUTBUF[1];
         p1 = EP1OUTBUF[2];
         SYNCDELAY;
         EP1OUTBC = 0;
         SYNCDELAY;
         CpldWriteReg(p0, p1);
         break;
	case CMD_CPLD_RD:
         p0 = EP1OUTBUF[1];
         EP1OUTBC = 0;
         SYNCDELAY;
         while(EP01STAT & bmBIT2);
         EP1INBUF[0] = CpldReadReg(p0);
         EP1INBC = 1;
         break;
  }
}

void main()
{
   UpInit();
   ClearDrivers();
   LedsOff();
   SetVoltages(79, 213, 1, 0);
   CpldWriteReg(CPLD_VMASK, 0);
   CpldWriteReg(CPLD_VCC_EN, 1);
   ClearDrivers();
   SetVoltages(1, 213, 1, 0);
//   FpgaAddr(0x0F);
//   FpgaWrite(0);
//   SetPinDrive(0x42, DRV_VPP);
//   LoadDrivers();

   while (1)
   {
      if ((EP01STAT & bmBIT1)==0)
      {
        Command();
      }
   }
}
