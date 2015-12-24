#ifndef _myDefine_h_
#define _myDefine_h_

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
void SYSTEMCLOCK_Init (void);
void PORT_Init (void);
void UART0_Init (void);
void UART1_Init (void);
INT8U KeyScan( void );

// Screen
void drawPage(int pageNum, int chipNum);
void drawRes(unsigned int r, int n);
void Delay(void);

// Mobile
int getInput();
void sendOutput(int, int, INT16U);

//Test
INT16U testChip(INT8U);
INT16U test00();
INT16U test04();
INT16U test11();
INT16U test20();
INT16U test27();
INT16U test86();
INT16U test125();
INT16U test74();
void fault74(INT8U id, INT16U *result);
void set74(INT8U *nS, INT8U *nR, INT8U *CP, INT8U *D, INT8U *Q, INT8U *nQ, INT8U id, INT16U result);
INT16U test75();
void set75( INT8U *CP,
            INT8U *D1, INT8U *Q1, INT8U *nQ1,
            INT8U *D2, INT8U *Q2, INT8U *nQ2,
            INT16U id, INT16U result);
void fault75(INT16U id, INT16U *result);
INT16U test85();
INT16U test253();
void set253(INT8U *A0, INT8U *A1, INT8U *D0, INT8U *D1, INT8U *D2, INT8U *D3, INT8U *nEN, INT8U *Y, INT8U id, INT16U result);
void fault253(INT8U id, INT16U *result);
INT16U test90();
void set90A(INT8U *R1, INT8U *R2, INT8U *S1, INT8U *S2,
            INT8U *Q0, INT8U *Q13,
            INT16U result);
void set90B(INT8U *R1, INT8U *R2, INT8U *S1, INT8U *S2,
            INT8U *Q0, INT8U *Q13,
            INT16U result);
void set90(INT8U *R1, INT8U *R2, INT8U *S1, INT8U *S2,
           INT8U *Q0, INT8U *Q13,
           INT8U nCPA, INT8U nCPB, INT16U result);
void set161(INT8U *in, INT8U *P, INT8U *T, INT8U *LD, INT8U *R, INT8U CP, INT8U *Q, INT8U *C0, INT16U result);
INT16U test161();
void set161CP(INT8U *in, INT8U *P, INT8U *T, INT8U *LD, INT8U *R, INT8U *Q, INT8U *C0, INT16U result);
void checkLED();


#endif