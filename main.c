//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "myDefine.h"
#include "mobile.h"
#include "myRingBuffer.h"
#include "test.h"
#include "init.h"
#include "draw.h"

//-----------------------------------------------------------------------------
// Global VARIABLES
//-----------------------------------------------------------------------------
bit UART = 0;                          // '0 is UART0; '1' is UART1
char c[4] = {0xFF, 0xFF, 0xFF}; 		//This is used at the end of eachinstruction to the screen
int pin_total[] = {14,14,14,14,14,14,14,16,16,14,14,14,16,16,14};	//number of pins of each chip


//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------
int main (void) {
	// mobile side
	int res = -1;
	struct RingBuffer *rb = init_RingBuffer();
    char p[20] = {'0','0','1','2','0','1','2',
    '0','1','2','0','1','2','0','1','2','0','1','2'};// START WITH P[1]
    int blueToothConnected = 9; // 10 blablabla
	// screen
	int chipNum = 0;
	INT16U page = 0;
	// returned result of the test
	INT16U result = 0;
    INT16U checking = 0;

	PCA0MD &= ~0x40;                    // Disable Watchdog timer
	SYSTEMCLOCK_Init ();                // initialize oscillator
	PORT_Init ();                       // initialize crossbar and GPIO
	UART0_Init ();                      // initialize UART0
	UART1_Init ();                      // initialize UART1

	P3 = P3 | 0x1F;
	UART = 0;
	printf("haha");
	updateBluetoothStatus(&blueToothConnected);
	if(!isBluetoothConnected(blueToothConnected)){
		printf("%s",c);
		printf("%s",c);
		drawPage(0,chipNum);
	}
	while (1) {
		updateBluetoothStatus(&blueToothConnected);
		if(isBluetoothConnected(blueToothConnected)){
			res = getInput(rb);
	        if (res > -1) {
				result = testChip(res);
	            sendOutput(res,pin_total[res],result);
	            res = -1;   
	        }else if(res == BLUETOOTH_CHANGE_NAME){
	        	changeName();
	        }else if(res == BLUETOOTH_CHANGE_PASS){
	        	changePass();
	        }else if(res == BLUETOOTH_RESET){
	        	bluetoothReset();
	        }
		}else{
			// scan Keys if bluetooth is not connected
			INT8U scan = KeyScan();
			// If simple hardware checking is working, we detect 'check stop' signal only.
			if (checking)
		    {
		     if (scan == K_CHECK)
		     {
		       checking = 0;
		       P3 = P3 | 0x1F;
		     }
		     checkLED();
		     continue;
		    }
			// set UART 0 so no message is sent by bluetooth.
			UART = 0;
			switch(scan)
			{
		        case K_CHECK:
		        checking = 1;
		        P3 = P3 & 0xE0;
		        break;
				case K1_RELEASE:
				// K1 means down button
					switch(page) 
					{
						//only at page 1(choose chip type) has function
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
				// K2 means  up button
					switch(page) 
					{
						//only at page 1(choose chip type) has function
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
				// K3 means ok button
					switch(page) 
					{
						case 0:
						// at page 0(welcome page), turn to page 1(choose page)
							page = 1;
							drawPage(page,chipNum);
							break;
						case 1:
						// at page 1(welcome page), turn to page 2(comfirm page)
							page = 2;
							drawPage(page,chipNum);
							break;
						case 2:
						// at page 2(confirm page), turn to page 3(result page) and start testing,show reslut on screen
							page = 3;
							drawPage(page,chipNum);
							result = testChip(chipNum);
							printf("%s",c);
							drawRes(result, chipNum);
							P3 = P3 | 0x1F;
							break;
						case 3:
						// at page 3(reslut page), turn to page 1(choose page)
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
