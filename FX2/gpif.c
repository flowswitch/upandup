// This program configures the General Programmable Interface (GPIF) for FX2.
// Parts of this program are automatically generated using the GPIFTool.
// Please do not modify sections of text which are marked as "DO NOT EDIT ...".
// You can modify the comments section of this GPIF program file using the dropdown menus
// and pop-up dialogs. These controls are available as hot spots in the text. Modifying the
// comments section will generate program code which will implement your GPIF program.
//
// DO NOT EDIT ...
// GPIF Initialization
// Interface Timing      Async
// Internal Ready Init   IntRdy=1
// CTL Out Tristate-able Binary
// SingleWrite WF Select     3
// SingleRead WF Select      2
// FifoWrite WF Select       1
// FifoRead WF Select        0
// Data Bus Idle Drive   Tristate
// END DO NOT EDIT

// DO NOT EDIT ...
// GPIF Wave Names
// Wave 0   = FIFORd
// Wave 1   = FIFOWr
// Wave 2   = SnglRd
// Wave 3   = SnglWr

// GPIF Ctrl Outputs   Level
// CTL 0    = P_RD     CMOS
// CTL 1    = P_WR     CMOS
// CTL 2    = CTL 2    CMOS
// CTL 3    = CTL 3    CMOS
// CTL 4    = CTL 4    CMOS
// CTL 5    = CTL 5    CMOS

// GPIF Rdy Inputs
// RDY0     = P_Empty
// RDY1     = P_Full
// RDY2     = RDY2
// RDY3     = RDY3
// RDY4     = RDY4
// RDY5     = RDY5
// FIFOFlag = FIFOFlag
// IntReady = IntReady
// END DO NOT EDIT
// DO NOT EDIT ...
//
// GPIF Waveform 0: FIFORd
//
// Interval     0         1         2         3         4         5         6     Idle (7)
//          _________ _________ _________ _________ _________ _________ _________ _________
//
// AddrMode Same Val  Same Val  Same Val  Same Val  Same Val  Same Val  Same Val
// DataMode NO Data   NO Data   Activate  NO Data   NO Data   NO Data   NO Data
// NextData SameData  SameData  SameData  SameData  SameData  SameData  SameData
// Int Trig No Int    No Int    No Int    No Int    No Int    No Int    Trig Int
// IF/Wait  IF        Wait 3    Wait 1    IF        Wait 1    Wait 1    IF
//   Term A P_Empty                       RDY2                          RDY2
//   LFunc  AND                           AND                           AND
//   Term B P_Empty                       RDY2                          RDY2
// Branch1  Then 1                        ThenIdle                      Then 6
// Branch0  Else 6                        ElseIdle                      Else 6
// Re-Exec  No                            No                            No
// Sngl/CRC Default   Default   Default   Default   Default   Default   Default
// P_RD         1         0         0         0         1         1         1         1
// P_WR         1         1         1         1         1         1         1         1
// CTL 2        1         1         1         1         1         1         1         1
// CTL 3        1         1         1         1         1         1         1         1
// CTL 4        1         1         1         1         1         1         1         1
// CTL 5        1         1         1         1         1         1         1         1
//
// END DO NOT EDIT
// DO NOT EDIT ...
//
// GPIF Waveform 1: FIFOWr
//
// Interval     0         1         2         3         4         5         6     Idle (7)
//          _________ _________ _________ _________ _________ _________ _________ _________
//
// AddrMode Same Val  Same Val  Same Val  Same Val  Same Val  Same Val  Same Val
// DataMode NO Data   Activate  NO Data   NO Data   NO Data   NO Data   NO Data
// NextData SameData  SameData  NextData  SameData  SameData  SameData  SameData
// Int Trig No Int    No Int    No Int    No Int    No Int    No Int    No Int
// IF/Wait  IF        Wait 3    Wait 1    IF        Wait 1    Wait 1    Wait 1
//   Term A P_Full                        RDY2
//   LFunc  AND                           AND
//   Term B P_Full                        RDY2
// Branch1  Then 1                        ThenIdle
// Branch0  Else 0                        ElseIdle
// Re-Exec  No                            No
// Sngl/CRC Default   Default   Default   Default   Default   Default   Default
// P_RD         1         1         1         1         1         1         1         1
// P_WR         1         0         1         1         1         1         1         1
// CTL 2        1         1         1         1         1         1         1         1
// CTL 3        1         1         1         1         1         1         1         1
// CTL 4        1         1         1         1         1         1         1         1
// CTL 5        1         1         1         1         1         1         1         1
//
// END DO NOT EDIT
// DO NOT EDIT ...
//
// GPIF Waveform 2: SnglRd
//
// Interval     0         1         2         3         4         5         6     Idle (7)
//          _________ _________ _________ _________ _________ _________ _________ _________
//
// AddrMode Same Val  Same Val  Same Val  Same Val  Same Val  Same Val  Same Val
// DataMode NO Data   NO Data   Activate  NO Data   NO Data   NO Data   NO Data
// NextData SameData  SameData  SameData  SameData  SameData  SameData  SameData
// Int Trig No Int    No Int    No Int    No Int    No Int    No Int    Trig Int
// IF/Wait  IF        Wait 3    Wait 1    IF        Wait 1    Wait 1    IF
//   Term A P_Empty                       RDY2                          RDY2
//   LFunc  AND                           AND                           AND
//   Term B P_Empty                       RDY2                          RDY2
// Branch1  Then 1                        ThenIdle                      Then 6
// Branch0  Else 6                        ElseIdle                      Else 6
// Re-Exec  No                            No                            No
// Sngl/CRC Default   Default   Default   Default   Default   Default   Default
// P_RD         1         0         0         0         1         1         1         1
// P_WR         1         1         1         1         1         1         1         1
// CTL 2        1         1         1         1         1         1         1         1
// CTL 3        1         1         1         1         1         1         1         1
// CTL 4        1         1         1         1         1         1         1         1
// CTL 5        1         1         1         1         1         1         1         1
//
// END DO NOT EDIT
// DO NOT EDIT ...
//
// GPIF Waveform 3: SnglWr
//
// Interval     0         1         2         3         4         5         6     Idle (7)
//          _________ _________ _________ _________ _________ _________ _________ _________
//
// AddrMode Same Val  Same Val  Same Val  Same Val  Same Val  Same Val  Same Val
// DataMode NO Data   Activate  NO Data   NO Data   NO Data   NO Data   NO Data
// NextData SameData  SameData  SameData  SameData  SameData  SameData  SameData
// Int Trig No Int    No Int    No Int    No Int    No Int    No Int    No Int
// IF/Wait  IF        Wait 3    Wait 1    IF        Wait 1    Wait 1    Wait 1
//   Term A P_Full                        RDY2
//   LFunc  AND                           AND
//   Term B P_Full                        RDY2
// Branch1  Then 1                        ThenIdle
// Branch0  Else 0                        ElseIdle
// Re-Exec  No                            No
// Sngl/CRC Default   Default   Default   Default   Default   Default   Default
// P_RD         1         1         1         1         1         1         1         1
// P_WR         1         0         1         1         1         1         1         1
// CTL 2        1         1         1         1         1         1         1         1
// CTL 3        1         1         1         1         1         1         1         1
// CTL 4        1         1         1         1         1         1         1         1
// CTL 5        1         1         1         1         1         1         1         1
//
// END DO NOT EDIT

// GPIF Program Code

// DO NOT EDIT ...
#include "fx2.h"
#include "fx2regs.h"
#include "fx2sdly.h"            // SYNCDELAY macro
#include "fx2macros.h"
// END DO NOT EDIT

// DO NOT EDIT ...
__xdata const char WaveData[128] =
{
// Wave 0
/* LenBr */ 1,   1,0x3F,   1,   1,   1,   1,   7,
/* Opcode*/ 0,   2,   5,   0,   0,   0,   0,   0,
/* Output*/ 2,   3,   3,   3,   3,   3,   3,   3,
/* LFun  */ 0,   0,   0,   0,   0,   0,   0,0x3F,
// Wave 1
/* LenBr */ 1,0x3F,   1,   1,   1,   1,   1,   7,
/* Opcode*/ 2,   5,   0,   0,   0,   0,   0,   0,
/* Output*/ 1,   3,   3,   3,   3,   3,   3,   3,
/* LFun  */ 0,   0,   0,   0,   0,   0,   0,0x3F,
// Wave 2
/* LenBr */ 1,   1,   1,0x3F,   1,   1,   1,   7,
/* Opcode*/ 1,   0,   2,   5,   0,   0,   0,   0,
/* Output*/ 3,   2,   3,   3,   3,   3,   3,   3,
/* LFun  */ 0,   0,   0,   0,   0,   0,   0,0x3F,
// Wave 3
/* LenBr */ 1,   1,0x3F,   1,   1,   1,   1,   7,
/* Opcode*/ 1,   2,   5,   0,   0,   0,   0,   0,
/* Output*/ 3,   1,   3,   3,   3,   3,   3,   3,
/* LFun  */ 0,   0,   0,   0,   0,   0,   0,0x3F
};
// END DO NOT EDIT

// DO NOT EDIT ...
__xdata const char InitData[7] =
{
/* Regs  */ 0xE0,0x10,0x00,0x03,0xEE,0x4E,0x00
};
// END DO NOT EDIT

// TO DO: You may add additional code below.

void GpifInit( void )
{
  BYTE i;

  // Registers which require a synchronization delay, see section 15.14
  // FIFORESET        FIFOPINPOLAR
  // INPKTEND         OUTPKTEND
  // EPxBCH:L         REVCTL
  // GPIFTCB3         GPIFTCB2
  // GPIFTCB1         GPIFTCB0
  // EPxFIFOPFH:L     EPxAUTOINLENH:L
  // EPxFIFOCFG       EPxGPIFFLGSEL
  // PINFLAGSxx       EPxFIFOIRQ
  // EPxFIFOIE        GPIFIRQ
  // GPIFIE           GPIFADRH:L
  // UDMACRCH:L       EPxGPIFTRIG
  // GPIFTRIG

  // Note: The pre-REVE EPxGPIFTCH/L register are affected, as well...
  //      ...these have been replaced by GPIFTC[B3:B0] registers

  // 8051 doesn't have access to waveform memories 'til
  // the part is in GPIF mode.

  IFCONFIG = 0xEE;
  // IFCLKSRC=1   , FIFOs executes on internal clk source
  // xMHz=1       , 48MHz internal clk rate
  // IFCLKOE=1    , Drive IFCLK pin signal at 48MHz
  // IFCLKPOL=0   , Don't invert IFCLK pin signal from internal clk
  // ASYNC=1      , master samples asynchronous
  // GSTATE=1     , Drive GPIF states out on PORTE[2:0], debug WF
  // IFCFG[1:0]=10, FX2 in GPIF master mode

  GPIFABORT = 0xFF;  // abort any waveforms pending

  GPIFREADYCFG = InitData[ 0 ];
  GPIFCTLCFG = InitData[ 1 ];
  GPIFIDLECS = InitData[ 2 ];
  GPIFIDLECTL = InitData[ 3 ];
  GPIFWFSELECT = InitData[ 5 ];
  GPIFREADYSTAT = InitData[ 6 ];

  // use dual autopointer feature...
  AUTOPTRSETUP = 0x07;          // inc both pointers,
                                // ...warning: this introduces pdata hole(s)
                                // ...at E67B (XAUTODAT1) and E67C (XAUTODAT2)

  // source
  APTR1H = MSB( &WaveData );
  APTR1L = LSB( &WaveData );

  // destination
  AUTOPTRH2 = 0xE4;
  AUTOPTRL2 = 0x00;

  // transfer
  for ( i = 0x00; i < 128; i++ )
  {
    EXTAUTODAT2 = EXTAUTODAT1;
  }

// Configure GPIF Address pins, output initial value,
  PORTCCFG = 0xFF;    // [7:0] as alt. func. GPIFADR[7:0]
  OEC = 0xFF;         // and as outputs
  PORTECFG |= 0x80;   // [8] as alt. func. GPIFADR[8]
  OEE |= 0x80;        // and as output

// ...OR... tri-state GPIFADR[8:0] pins
//  PORTCCFG = 0x00;  // [7:0] as port I/O
//  OEC = 0x00;       // and as inputs
//  PORTECFG &= 0x7F; // [8] as port I/O
//  OEC &= 0x7F;      // and as input

// GPIF address pins update when GPIFADRH/L written
  SYNCDELAY;                    //
  GPIFADRH = 0x00;    // bits[7:1] always 0
  SYNCDELAY;                    //
  GPIFADRL = 0x00;    // point to PERIPHERAL address 0x0000
}
