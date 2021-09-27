#ifndef _CPLD_H_INCLUDED
#define _CPLD_H_INCLUDED

#include "cpldregs.h"

//pin drive modes
#define DRV_GND 0
#define DRV_VCC 1
#define DRV_VPP 2
#define DRV_OFF 0xFF

//voltage pwm values
#define VIO_3V3 0xD5
#define VCC_3V3 0x4F

extern void mdelay(BYTE dly);
extern void UpInit(void);
extern void CpldAddr(unsigned char);
extern void CpldWrite(unsigned char);
extern unsigned char CpldRead(void);
extern void CpldWriteReg(unsigned char, unsigned char);
extern unsigned char CpldReadReg(unsigned char addr);

extern void ApplyPinDrivers(void);
extern void DisablePinDrivers(void);
extern void ResetPinConfig(void);
extern void SetPinDrive(unsigned char pin, unsigned char type);

extern void SetVoltages(unsigned char vcc, unsigned char vio, unsigned char vpp, unsigned char vmask);
extern void SetVcc(unsigned char pwm);
extern void SetVio(unsigned char pwm);
extern void SetVpp(unsigned char pwm);
extern BYTE RampVcc();


extern void LedsOff(void);
extern void LedGreen(void);
extern void LedRed(void);
extern void Beep(void);

extern void SendStatus(unsigned char sts);

#endif
