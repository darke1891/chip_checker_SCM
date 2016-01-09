#include "myDefine.h"
#include "myRingBuffer.h"
#include <string.h>   

char buffer[100];
int bufferLength = 0;

int mydebug = 0;
int syncCount;

char getchar2();
int proceedBuffer();
int proceedString(int);

int getInput();
void sendOutput(int, int, INT16U);

int getNumber(int);
int findSemicolon();
void bufferToleft(int);

void mysleep(int time);
void myprintint(int n,int t);
void myprintchar(char c,int t);


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
		UART=0;
		printf("haha%dahah",pin_result);
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

int getInput (void) {

    char input_char;
    int returnResult = 0;
    UART = 1;                
    input_char = getchar2();
    if(input_char >='#' && input_char <= 'z'){
        buffer[bufferLength++] = input_char;
        buffer[bufferLength] = 0;
    }
    return proceedBuffer();
}

char getchar2() {
    char c;
    if (SCON1 & 0x01) {
        c = SBUF1;
        SCON1 &= ~0x01;
    }else
        c = 1;
    return  c;
}

int proceedBuffer(){// change later
    int rank = -2;
    int returnres;
    while(rank != -1){
        int rank = findSemicolon();
        if(rank == -1){
            return -1;
        }
        returnres =  proceedString(rank);
        return returnres;
    }   
}
/** */
int proceedString(int rank){
    int returnResult = -1,chipNum;
    UART = 0;
    if(buffer[0] == '#'){
		UART = 0;
		printf("SOF[bef:%s]EOF\n",buffer);
        bufferToleft(1);
        rank --;
        syncCount = getNumber(rank);
        returnResult = -1;
    }else if(buffer[0] == '@'){
		UART = 0;
		printf("SOF[bef:%s]EOF\n",buffer);
        bufferToleft(1);
        rank --;
        chipNum = getNumber(rank);
        returnResult = chipNum;
    }else if(buffer[0] == 'n'){
        UART = 0;
        printf("NAMESOF[bef:%s]EOF\n",buffer);
        bufferToleft(1);
        rank --;
        returnResult = -2;
    }else if(buffer[0] == 'm'){
        UART = 0;
        printf("PASSSOF[bef:%s]EOF\n",buffer);
        bufferToleft(1);
        rank --;
        returnResult = -3;
    }
        bufferToleft(rank+1);
        return returnResult;
}

int findSemicolon(){
    int i;
    for(i = 0;i < bufferLength;i++){
        if(buffer[i] == ';'){
            return i;
        }
    }  
    return -1;
}

int getNumber(int rank){
    int result = 0,i;
    for(i = 0;i< rank;i++){
		UART = 0;
		//printf("SOF[buffer:%s]EOF\n",buffer);
		//printf("SOF[rank%d]EOF\n",rank);
		//printf("SOF[get:%c]EOF\n",buffer[i]);
        if(buffer[i]>'9' || buffer[i] < '0'){
            return -1;
    	}
    	result = result * 10 + buffer[i]-'0';
    }
    return result;
}

void bufferToleft(int n){
    int i;
    for(i=0;i<bufferLength-n;i++){ //bufferLength>1
        buffer[i] = buffer[i+1];
    }bufferLength = bufferLength-n;
}

void mysleep(int time) {
    int x1,x2;
    for(x2 = 0;x2 < time;x2){
        for(x1 = 0;x1 < 20000;x1)//4secs
            x1++;
        x2++;
    }
}

void myprintint(int n,int t) {
    int a = 0;
    if(n>9){
        printf("%d",n/10);
        mysleep(t);
    }
    printf("%d",n%10);
	mysleep(t);
}

void myprintchar(char c,int t) {
    putchar(c);
	mysleep(t);
}