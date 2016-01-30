#include "myDefine.h"

void mysleep(int time) 
{
    int x1,x2;
    for(x2 = 0;x2 < time;x2){
        for(x1 = 0;x1 < 20000;x1)
            x1++;
        x2++;
    }
}

void Delay(void)
{
  int x;
  for(x = 0;x < 500;x)
  {
    x++;
  }
}
//-----------------------------------------------------------------------------
// putchar
//-----------------------------------------------------------------------------
//
// Return Value : UART0/1 buffer value
// Parameters   : character to be transmitted across UART0/1
//
// This is an overloaded fuction found in the stdio library.  When the
// function putchar is called, either by user code or through calls to stdio
// routines such as printf, the following routine will be executed instead 
// of the function located in the stdio library.
//
// The function checks the UART global variable to determine which UART to 
// use to receive a character.
//
// The routine expands '\n' to include a carriage return as well as a 
// new line character by first checking to see whether the character  
// passed into the routine equals '\n'.  If it is, the routine waits for 
// TI0/TI1 to be set, indicating that UART 0/1 is ready to transmit another 
// byte.  The routine then clears TI0/TI1 bit, and sets the UART0/1 output 
// buffer to '0x0d', which is the ASCII character for carriage return.
//
// The routine the waits for TI0/TI1 to be set, clears TI0/TI1, and sets
// the UART output buffer to <c>.  
//
//-----------------------------------------------------------------------------
char putchar (char c)  {

   if (UART == 0) {

      if (c == '\n')  {                // check for newline character
         while (!TI0);                 // wait until UART0 is ready to transmit
         TI0 = 0;                      // clear interrupt flag
         SBUF0 = 0x0d;                 // output carriage return command
      }
      while (!TI0);                    // wait until UART0 is ready to transmit
      TI0 = 0;                         // clear interrupt flag
      return (SBUF0 = c);              // output <c> using UART 0
   }

   else if (UART == 1) {
      if (c == '\n')  {                // check for newline character
         while (!(SCON1 & 0x02));      // wait until UART1 is ready to transmit
         SCON1 &= ~0x02;               // clear TI1 interrupt flag
         SBUF1 = 0x0d;                 // output carriage return
      }
      while (!(SCON1 & 0x02));         // wait until UART1 is ready to transmit
      SCON1 &= ~0x02;                  // clear TI1 interrupt flag
      return (SBUF1 = c);              // output <c> using UART 1
   }
}

//-----------------------------------------------------------------------------
// _getkey
//-----------------------------------------------------------------------------
//
// Return Value : byte received from UART0/1
// Parameters   : none

// This is an overloaded fuction found in the stdio library.  When the
// function _getkey is called, either by user code or through calls to stdio
// routines such as scanf, the following routine will be executed instead 
// of the function located in the stdio library.
//
// The function checks the UART global variable to determine which UART to 
// use to receive a character.
//
// The routine waits for RI0/RI1 to be set, indicating that a byte has
// been received across the UART0/UART1 RX line.  The routine saves the 
// received character into a local variable, clears the RI0/RI1 interrupt
// flag, and returns the received character value.
//
//-----------------------------------------------------------------------------
char _getkey ()  {
  char c;

  if (UART == 0) {
    while (!RI0);                      // wait until UART0 receives a character
    c = SBUF0;                         // save character to local variable
    RI0 = 0;                           // clear UART0 receive interrupt flag
    return (c);                        // return value received through UART0
  }

  else if (UART == 1) {
    while (!(SCON1 & 0x01));           // wait until UART1 receives a character
    c = SBUF1;                         // save character to local variable
    SCON1 &= ~0x01;                    // clear UART1 receive interrupt flag
    return (c);                        // return value received through UART1
  }
}
