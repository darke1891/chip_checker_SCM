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
char c[4] = {0xFF, 0xFF, 0xFF}; 
int pin_total[] = {14,14,14,14,14,14,14,16,16,14,14,14,16,16,14};


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
			INT8U scan = KeyScan();
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
			UART = 0;
			switch(scan)
			{
		        case K_CHECK:
		        checking = 1;
		        P3 = P3 & 0xE0;
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
							P3 = P3 | 0x1F;
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
