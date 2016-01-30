#include "mobile.h"
#include "myDefine.h"
#include "myRingBuffer.h"
#include <C8051F340.h>
#include <string.h> 
#include <stdlib.h>  

/** To keep track of syncing. */
int syncCount;


/** Due to its incapability of sending buffer too fast,
 *  this function is implemented to print int with time leap.
 *  @params n: buffer int
 *  @params t: time leap
 */
void myprintint(int n,int t) 
{
    int a = 0;
    if(n>9){
        printf("%d",n/10);
        mysleep(t);
    }
    printf("%d",n%10);
    mysleep(t);
}


/** Due to its incapability of sending buffer too fast,
 *  this function is implemented to print int with time leap.
 *  @params c: buffer char
 *  @params t: time leap
 */
void myprintchar(char c,int t) 
{
    putchar(c);
    mysleep(t);
}


/** Getting input from bluetooth. 
 *  No stuck implementation(scanf waits for '\n').
 *  @return: 1 if no input, else the char value read from buffer.
 */
char getchar2()
{
    char c;
    UART = 1;
    if (SCON1 & 0x01) {
        c = SBUF1;
        SCON1 &= ~0x01;
    }else
        c = 1;
    return  c;
}


/** */
int proceedString(char const *s,int rank)
{
    int returnResult = -1;
    if(s[0] == '#'){       // sync check
        syncCount = atoi(s+1);
        returnResult = -1;
    }else if(s[0] == '@'){ // return chip number
        returnResult = atoi(s+1);
    }else if(s[0] == 'n'){ // change name
        returnResult = BLUETOOTH_CHANGE_NAME;
    }else if(s[0] == 'm'){ // change password
        returnResult = BLUETOOTH_CHANGE_PASS;
    }else if(s[0] == 'r'){ // reset
        returnResult = BLUETOOTH_RESET;
    }
    return returnResult;
}


/***/
int proceedBuffer(struct RingBuffer* rb)
{
    int rank = -2; // avoid -1
    int returnres;
    char s[30];
    while(rank != -1){
        rank = find_RingBuffer(rb, ';');
        if(rank == -1){
            return -1;
        }else{
            if(pop_RingBuffer(rb,s,rank+1)){
                s[rank] = 0;
                UART = 0;printf("%s\n", s);
            }//else log pop error
        }
        returnres =  proceedString(s,rank);
        return returnres;
    }   
}

/** No stuck get input function.
 *  Reads and proceeds from buffer(and stores incomplete 
 *  messages).
 *  @params rb: global ring buffer structure pointer.
 */
int getInput (struct RingBuffer* rb) 
{
    char inputBuffer[35];
    int maxlen = 30,index = 0;
    char input_char = getchar2();
    memset( inputBuffer, 0, sizeof(char)*maxlen );
    while(input_char != 1 && index < maxlen){
        // To filter some unwanted chars
        if(input_char >='#' && input_char <= 'z'){
            inputBuffer[index++] = input_char;
            input_char = getchar2();
        }
    }
    write_RingBuffer(rb,inputBuffer);
    return proceedBuffer(rb);
}


/** Update bluetooth connection value.
 *  @params bt: value for connection status. connected(==10) or not(>=0&&<10) 
 */
void updateBluetoothStatus(int * const bt)
{
    if((P3>>7) & 1){
        if(*bt < 10) *bt = *bt + 1;
    }else {
        *bt = 0;
    }
}


/** Check connection status.
 *  @return bt: connected(1) or not(0)
 */
int isBluetoothConnected(int bt)
{
    if(bt >= 10) // '==' should be ok
        return 1;
    else 
        return 0;
}


void sendOutput(int chipNum,int n,INT16U result) {
    int mpause = 25;int i; int count_pin=0;
    int pin_result;
    char out,lastchar=0;
    UART = 1;
    myprintchar('#',mpause);
    myprintint(syncCount,mpause);
    myprintchar(';',mpause);
    myprintchar('@',mpause);
    myprintint(chipNum,mpause);
    myprintchar(';',mpause);     
    for(i=1;i<n+1;i++){
        if(n == 14 && i>=8)
            pin_result = ((result >> (i+1)) & 1);
        else
            pin_result = ((result >> (i-1)) & 1);
		UART=1;
        out = pin_result + '0';
		if(out != lastchar && lastchar != 0){
			myprintchar('p',mpause);
	        myprintchar(lastchar,mpause);
	        myprintchar('s',mpause);
			myprintint(count_pin,mpause);
	        myprintchar(';',mpause);
			count_pin = 0;
		}lastchar = out;count_pin++;
    }
	myprintchar('p',mpause);
	myprintchar(out,mpause);
	myprintchar('s',mpause);
	myprintint(count_pin,mpause);
	myprintchar(';',mpause);
}

void changeName()
{
	 UART = 1;
	printf("AT+NAMEjinjiayu");
}

void changePass()
{
	  UART = 1;
	printf("AT+PINjinjiayu");
}

void bluetoothReset()
{
	   UART = 1;
	printf("AT+NAMEHC-06");
	printf("AT+PIN1234");
}
