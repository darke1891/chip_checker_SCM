//-----------------------------------------------------------------------------
// F34x_UART_MultiUART.c
//-----------------------------------------------------------------------------
// Copyright 2006 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// This program configures UART0 and UART1 to operate in polled mode, suitable
// for use with the stdio.h functions printf() and scanf().
//
// Test code is included for printf() and getchar(), and shows an example
// of how the putchar() and _getkey() library routines can be overridden to
// allow for multiple UARTs to be selected by the functions (in this example, 
// the global variable UART is used to select between UART0 and UART1).
//
// The example sets system clock to maximum frequency of 48 MHz.
//
// The system clock frequency is stored in a global constant SYSCLK.  The
// target UART0 baud rate is stored in a global constant BAUDRATE0, and the
// target UART1 baud rate is stored in a global constant BAUDRATE1.
//
//
// How To Test:
//
// 1) Download code to a 'F34x device that is connected to a UART transceiver
// 2) Connect serial cable from the transceiver to a PC
// 3) On the PC, open HyperTerminal (or any other terminal program) and connect
//    to the COM port at <BAUDRATE0>/<BAUDRATE1> and 8-N-1
// 4) To test UART 0, place jumpers connecting P0.4 to TX and P0.5 to RX on 
//    C8051F340 Target Board header J12.  To test UART 1, run wires from
//    P0.0 on J2 to the TX pin of J12, and P0.1 on J2 to the RX pin of J12.
//
//
// FID:          34X000081  
// Target:       C8051F34x
// Tool chain:   KEIL C51 7.20 / KEIL EVAL C51
//               Silicon Laboratories IDE version 2.72
//
// Release 1.0
//    -Initial Revision (PD)
//    -17 JUL 2006
//    -Initial Release
//
//
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include "myDefine.h"

//-----------------------------------------------------------------------------
// Global VARIABLES
//-----------------------------------------------------------------------------

bit UART = 0;                          // '0 is UART0; '1' is UART1
char c[4] = {0xFF, 0xFF, 0xFF}; 
int pin_total[] = {14,14,14,14,14,14,14,16,16,14,14,14,16,16,14};


//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------

int main (void) {
	// mobile side
	int res = -1;
    char p[20] = {'0','0','1','2','0','1','2',
    '0','1','2','0','1','2','0','1','2','0','1','2'};// START WITH P[1]
    
	//
	char input_char;
	int chipNum = 0;
	int blueToothConnected = 1;
	INT16U page = 0;
	// returned result of the test
	INT16U result = 0;
    INT16U checking = 0;

	PCA0MD &= ~0x40;                    // Disable Watchdog timer
	SYSTEMCLOCK_Init ();                // initialize oscillator
	PORT_Init ();                       // initialize crossbar and GPIO
	UART0_Init ();                      // initialize UART0
	UART1_Init ();                      // initialize UART1

	P3 = P3 & 0xC0;
	UART = 0;
	if(blueToothConnected == 0){
		printf("%s",c);
		printf("%s",c);
		drawPage(0,chipNum);
	}
	while (1) {
		if(blueToothConnected){
			UART = 1;
			res = getInput();
	        if (res != -1) {
	            UART = 0;
				result = testChip(res);
				UART = 0;
				printf("%d,%d,%x\n",res,pin_total[res],result);
	            sendOutput(res,pin_total[res],result);
	            res = -1;   
	        }
		}else{
			INT8U scan = KeyScan();
		    if (checking)
		    {
		     if (scan == K_CHECK)
		     {
		       checking = 0;
		       P3 = P3 & 0xE0;
		       P4 = P4 & 0xFC;
		     }
		     checkLED();
		     continue;
		    }
			UART = 0;
			switch(scan)
			{
		        case K_CHECK:
		        checking = 1;
		        P3 = P3 | 0x1F;
		        P4 = P4 | 0x3;
		        break;
				case K1_RELEASE:
					switch(page) 
					{
						case 1:
							if(chipNum == 0)
								chipNum = 14;
							else
								chipNum -= 1;
							drawPage(page,chipNum);
							break;
						default:
							break;

					}
					break;
				case K2_RELEASE:
					switch(page) 
					{
						case 1:
							if(chipNum == 14)
								chipNum = 0;
							else
								chipNum += 1;
							drawPage(page,chipNum);
							break;
						default:
							break;

					}
					break;
				case K3_RELEASE:
					switch(page) 
					{
						case 0:
							page = 1;
							drawPage(page,chipNum);
							break;
						case 1:
							page = 2;
							drawPage(page,chipNum);
							break;
						case 2:
							page = 3;
							drawPage(page,chipNum);
							result = testChip(chipNum);
							printf("%s",c);
							drawRes(result, chipNum);
							P3 = P3 & 0xC0;
							break;
						case 3:
							page = 1;
							drawPage(page,chipNum);
						default:
							break;
					}
					break;
				default:
					break;
			}
		}
	}
	return 0;
}
