#ifndef __MY_MOBILE_H__
#define __MY_MOBILE_H__
#include "myRingBuffer.h"

#define BLUETOOTH_CHANGE_NAME -2
#define BLUETOOTH_CHANGE_PASS -3
#define BLUETOOTH_RESET       -4

int getInput(struct RingBuffer*);

void sendOutput(int, int, INT16U);

/** Update bluetooth connection value.
 *  @params bt: value for connection status. connected(==10) or not(>=0&&<10) 
 */
void updateBluetoothStatus(int * const);
/** Check connection status.
 *  @return bt: connected(1) or not(0)
 */
int isBluetoothConnected(int);

void changeName();
void changePass();
void bluetoothReset();

#endif