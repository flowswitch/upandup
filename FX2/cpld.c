#include "string.h"
//#include "fx2.h"
#include "fx2regs.h"
#include "fx2sdly.h"            // SYNCDELAY macro
#include "cpld.h"
#include "ports.h"

//256 pin drivers (GND/Vcc/Vpp each) are organized into 8 rows of 32 pins
//each row is a shift reg: Dx -> G0-31 -> Vcc0-31 -> Vpp0-31  
#define N_PIN_ROWS      8
#define PINS_PER_ROW    32
#define BYTES_PER_ROW   (PINS_PER_ROW>>3)
#define N_DRV_TYPES     3
__xdata static unsigned char DrvCfg[N_PIN_ROWS][N_DRV_TYPES][BYTES_PER_ROW];

static BYTE outport=0;

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

/////// CLPD bus accesses ///////////////

void CpldAddr(unsigned char addr)
{
    IOD = addr | CPLD_ADDR_MASK | CPLD_nRD_MASK | CPLD_nWR_MASK;
    IO_CPLD_nWR = 0;
    IO_CPLD_nWR = 1;
}

void CpldWrite(unsigned char val)
{
    IOD = val | CPLD_nRD_MASK | CPLD_nWR_MASK;
    IO_CPLD_nWR = 0;
    IO_CPLD_nWR = 1;
}

unsigned char CpldRead(void)
{
    unsigned char val;
    OED = CPLD_ADDR_MASK | CPLD_nRD_MASK | CPLD_nWR_MASK;
    IOD = CPLD_nWR_MASK;
    val = IOD & 0x1F;
    IO_CPLD_nRD = 1;
    OED = 0xFF;
    return val;
}

void CpldWriteReg(unsigned char addr, unsigned char val)
{
    if (addr==CPLD_REG_PUPD)
    {
        CpldAddr(CPLD_REG_PUPD2);
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

////////// Pin drivers ///////////////////

void ApplyPinDrivers(void)
{
    register unsigned char t,i,b,cf0,cf1,cf2,cf3,outval;

    CpldWriteReg(CPLD_REG_DRVCTL, 0); //deassert STB, OE

    //load drivers 0, 1
    CpldAddr(CPLD_REG_DRV01DAT);
    t = N_DRV_TYPES;
    do
    {
        t--;
        i = BYTES_PER_ROW;
        do
        {
            i--;
            cf0 = DrvCfg[0][t][i];
            cf1 = DrvCfg[1][t][i];
            cf2 = DrvCfg[2][t][i];
            cf3 = DrvCfg[3][t][i];
            for(b=0; b<8; b++)
            {
                outval = CPLD_nRD_MASK;
                if (cf0 & 0x80)
                    outval |= CPLD_DRVDAT_D0;
                if (cf1 & 0x80)
                    outval |= CPLD_DRVDAT_D1;
                if (cf2 & 0x80)
                    outval |= CPLD_DRVDAT_D2;
                if (cf3 & 0x80)
                    outval |= CPLD_DRVDAT_D3;
                IOD = outval; //CLK low
                IOD = outval | CPLD_DRVDAT_CLK; //CLK high
                cf0<<=1;
                cf1<<=1;
                cf2<<=1;
                cf3<<=1;
            }
        } while(i!=0);
    } while(t!=0);

    //load drivers 2, 3
    CpldAddr(CPLD_REG_DRV23DAT);
    t = N_DRV_TYPES;
    do
    {
        t--;
        i = BYTES_PER_ROW;
        do
        {
            i--;
            cf0 = DrvCfg[4][t][i];
            cf1 = DrvCfg[5][t][i];
            cf2 = DrvCfg[6][t][i];
            cf3 = DrvCfg[7][t][i];
            for(b=0; b<8; b++)
            {
                outval = CPLD_nRD_MASK;
                if (cf0 & 0x80)
                    outval |= CPLD_DRVDAT_D0;
                if (cf1 & 0x80)
                    outval |= CPLD_DRVDAT_D1;
                if (cf2 & 0x80)
                    outval |= CPLD_DRVDAT_D2;
                if (cf3 & 0x80)
                    outval |= CPLD_DRVDAT_D3;
                IOD = outval; //CLK low
                IOD = outval | CPLD_DRVDAT_CLK; //CLK high
                cf0<<=1;
                cf1<<=1;
                cf2<<=1;
                cf3<<=1;
            }
        } while(i!=0);
    } while(t!=0);

    CpldAddr(CPLD_REG_DRVCTL);
    //pulse STB to latch the regs
    IOD = CPLD_nRD_MASK | CPLD_DRVCTL_STB;
    IOD = CPLD_nRD_MASK;
    //enable reg outputs
    outval = IO_CPLD_ERR ? CPLD_DRVCTL_OE0145 | CPLD_DRVCTL_OE2367 : 0; //OE
    IOD = outval | CPLD_nRD_MASK;
    IOD = outval | CPLD_nRD_MASK | CPLD_nWR_MASK;
}

void DisablePinDrivers(void)
{
    memset(DrvCfg, 0, sizeof(DrvCfg));
    ApplyPinDrivers();
    CpldWriteReg(CPLD_REG_PUPD, CPLD_PUPD_PD);
    mdelay(2);
}

void SetPinDrive(unsigned char pin, unsigned char type)
{
    // pin.7:4 - row, pin.4:3 - byte, pin.2:0 - bit
    // pre-clear
    register BYTE t;
    for(t=0; t<N_DRV_TYPES; t++)
    {
        DrvCfg[pin>>5][t][(pin>>3) & 3] &= ~(1<<(pin & 7));
    }
    if (type<N_DRV_TYPES) //supplying an invalid type will leave this pin driver disabled
    {
        DrvCfg[pin>>5][type][(pin>>3) & 3] |= 1<<(pin & 7);
    }
}

void ResetPinConfig(void)
{
    memset(DrvCfg, 0, 12*8);
}

void SetVcc(unsigned char pwm)
{
    CpldWriteReg(CPLD_REG_VCC_L, pwm & 0x0F);
    CpldWriteReg(CPLD_REG_VCC_H, pwm>>4);
}

//////// Voltage control //////////////////

void SetVio(unsigned char pwm)
{
    CpldWriteReg(CPLD_REG_VIO_L, pwm & 0x0F);
    CpldWriteReg(CPLD_REG_VIO_H, pwm>>4);
}

void SetVpp(unsigned char pwm)
{
    CpldWriteReg(CPLD_REG_VPP_L, pwm & 0x0F);
    CpldWriteReg(CPLD_REG_VPP_H, pwm>>4);
}

void SetVoltages(unsigned char vcc, unsigned char vio, unsigned char vpp, unsigned char vmask)
{
    if (vcc>115)
        vmask |= CPLD_DCDCRANGE_VCC;
    if (vpp>177)
        vmask |= CPLD_DCDCRANGE_VPP;
    if (vcc!=1)
        CpldWriteReg(CPLD_REG_VCCEN, CPLD_VCCEN);
        CpldWriteReg(CPLD_REG_DCDCRANGE, vmask);
    SetVcc(vcc);
    SetVio(vio);
    SetVpp(vpp);
    mdelay(10);
}

BYTE RampVcc()
{
    BYTE p0;
    DisablePinDrivers();
    SetVcc(20);
    for(p0=20; p0<58; p0++)
    {
        SetVcc(p0);
        mdelay(2);
        if (CpldReadReg(0x10) & 1)
        {
            break;
        }
    }
    SetVcc(VCC_3V3);
    return p0;
}

///////// Indication ////////////////

void LedsOff()
{
    outport = 0;
    CpldWriteReg(CPLD_REG_GPO, 0);
}

void LedGreen()
{
    outport |= CPLD_GPO_LED_G;
    CpldWriteReg(CPLD_REG_GPO, outport);
}

void LedRed()
{
    outport |= CPLD_GPO_LED_R;
    CpldWriteReg(CPLD_REG_GPO, outport);
}

void Beep()
{
    register BYTE i,j;

    CpldAddr(CPLD_REG_BUZZER);
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
    CpldAddr(CPLD_REG_VCC_L);
    EP1INBUF[1] = IO_CPLD_ERR;
    SYNCDELAY;
    EP1INBC = 2;
}

////////// Misc ////////////////////

void ReadSN(void)
{
    register BYTE i;
    while(EP01STAT & bmBIT2);
    for(i=0; i<4; i++)
    {
        EP1INBUF[i] = (CpldReadReg(CPLD_REG_SN0+(i<<1)) & 0x0F) | ((CpldReadReg(CPLD_REG_SN1+(i<<1)) & 0x0F)<<4);
    }
    SYNCDELAY;
    EP1INBC = 4;
}
