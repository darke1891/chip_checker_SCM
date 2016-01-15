#ifndef __MYDEFINE_H__
#define __MYDEFINE_H__

#include <C8051F340.h>                 // SFR declarations
#include <stdio.h>
#include <stdlib.h>

typedef unsigned char  INT8U;
typedef unsigned int INT16U;
extern char c[4];
#define K_NONE           0x00
#define K1_PRESS         0x01
#define K2_PRESS         0x02
#define K3_PRESS         0x03
#define K1_RELEASE       0x10
#define K2_RELEASE       0x20
#define K3_RELEASE       0x30
#define K_CHECK			 0x40

extern bit UART;


/** Sleep function.
 *  @params time: real time leap unknown.
*/
void mysleep(int time);

void Delay(void);

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
char putchar (char c);

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
char _getkey ();

#endif
