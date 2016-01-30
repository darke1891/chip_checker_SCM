#include "init.h"
#include "myDefine.h"

//-----------------------------------------------------------------------------
// 16-bit SFR Definitions for 'F34x
//-----------------------------------------------------------------------------

sfr16 SBRL1 = 0xB4;

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

#define SYSCLK       48000000          // SYSCLK frequency in Hz
//#define BAUDRATE0     115200           // Baud rate of UART0 in bps
//#define BAUDRATE1     115200           // Baud rate of UART1 in bps
#define BAUDRATE0     9600           // Baud rate of UART0 in bps
#define BAUDRATE1     9600           // Baud rate of UART1 in bps



//-----------------------------------------------------------------------------
// Initialization Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// SYSTEMCLOCK_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None

// This routine initializes the system clock to use the internal system clock
// routed through the clock multiplier as its clock source.
//
//-----------------------------------------------------------------------------

void SYSTEMCLOCK_Init (void)
{
   OSCICN |= 0x03;                     // Configure internal oscillator for
                                       // its maximum frequency and enable
                                       // missing clock detector

   CLKMUL  = 0x00;                     // Select internal oscillator as
                                       // input to clock multiplier

   CLKMUL |= 0x80;                     // Enable clock multiplier
   Delay();                            // Delay for clock multiplier to begin
   CLKMUL |= 0xC0;                     // Initialize the clock multiplier
   Delay();                            // Delay for clock multiplier to begin

   while(!(CLKMUL & 0x20));            // Wait for multiplier to lock
   CLKSEL  = 0x03;                     // Select system clock
}

//-----------------------------------------------------------------------------
// PORT_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None

// Configure the Crossbar and GPIO ports
//
// P0.0     TX1 (UART 1)
// P0.1     RX1 (UART 1)
// P0.2     
// P0.3
// P0.4     TX0 (UART 0)
// P0.5     RX0 (UART 0)
// P0.6
// P0.7

// P1       not used in this example

// P2.0
// P2.1
// P2.2     LED
// P2.3    
// P2.4
// P2.5
// P2.6
// P2.7
//-----------------------------------------------------------------------------

void PORT_Init (void)
{  
  P0SKIP    = 0x03;
  XBR0 = 0x01;                        // route UART 0 to crossbar
  XBR2 = 0x01;                        // route UART 1 to crossbar
  XBR1 = 0x40;                        // enable crossbar
  P0MDOUT |= 0x11;                    // set P0.4 and P0.5 to push-pull output
  P3MDOUT = 0x3F;                     // set P3.0 - P3.6 to be ready for output
  P1MDOUT = 0x0;
  P2MDOUT = 0x0;
}

//-----------------------------------------------------------------------------
// UART0_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Configure the UART0 using Timer1, for baudrate; and 8-N-1.
//
//-----------------------------------------------------------------------------
void UART0_Init (void)
{
   SCON0 = 0x10;                       // SCON0: 8-bit variable bit rate
                                       //        level of STOP bit is ignored
                                       //        RX enabled
                                       //        ninth bits are zeros
                                       //        clear RI0 and TI0 bits

   if (SYSCLK/BAUDRATE0/2/256 < 1) {
      TH1 = -(SYSCLK/BAUDRATE0/2);
      CKCON &= ~0x0B;                  // T1M = 1; SCA1:0 = xx
      CKCON |=  0x08;
   } else if (SYSCLK/BAUDRATE0/2/256 < 4) {
      TH1 = -(SYSCLK/BAUDRATE0/2/4);
      CKCON &= ~0x0B;                  // T1M = 0; SCA1:0 = 01                 
      CKCON |=  0x09;
   } else if (SYSCLK/BAUDRATE0/2/256 < 12) {
      TH1 = -(SYSCLK/BAUDRATE0/2/12);
      CKCON &= ~0x0B;                  // T1M = 0; SCA1:0 = 00
   } else {
      TH1 = -(SYSCLK/BAUDRATE0/2/48);
      CKCON &= ~0x0B;                  // T1M = 0; SCA1:0 = 10
      CKCON |=  0x02;
   }

   TL1 = TH1;                          // init Timer1
   TMOD &= ~0xf0;                      // TMOD: timer 1 in 8-bit autoreload
   TMOD |=  0x20;                       
   TR1 = 1;                            // START Timer1
   TI0 = 1;                            // Indicate TX0 ready
}

//-----------------------------------------------------------------------------
// UART1_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Configure UART1 for baudrate1 and 8-N-1.
//
//-----------------------------------------------------------------------------

void UART1_Init (void)
{
   SMOD1 = 0x0C;                       // set to disable parity, 8-data bit,
                                       // disable extra bit, 
                                       // stop bit 1 bit wide

   SCON1 = 0x10;                       // SCON1: 8-bit variable bit rate
                                       //        level of STOP bit is ignored
                                       //        RX enabled
                                       //        ninth bits are zeros
                                       //        clear RI0 and TI0 bits

   if (SYSCLK/BAUDRATE1/2/0xFFFF < 1) {
      SBRL1 = -(SYSCLK/BAUDRATE1/2);
      SBCON1 |= 0x03;                  // set prescaler to 1
   } else if (SYSCLK/BAUDRATE1/2/0xFFFF < 4) {
      SBRL1 = -(SYSCLK/BAUDRATE1/2/4);
      SBCON1 &= ~0x03;
      SBCON1 |= 0x01;                  // set prescaler to 4

   } else if (SYSCLK/BAUDRATE1/2/0xFFFF < 12) {
      SBRL1 = -(SYSCLK/BAUDRATE1/2/12);
      SBCON1 &= ~0x03;                 // set prescaler to 12
   } else {
      SBRL1 = -(SYSCLK/BAUDRATE1/2/48);
      SBCON1 &= ~0x03;
      SBCON1 |= 0x02;                  // set prescaler to 4
   }

   SCON1 |= 0x02;                      // indicate ready for TX
   SBCON1 |= 0x40;                     // enable baud rate generator
}
