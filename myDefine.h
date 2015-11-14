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
extern bit UART;
void SYSTEMCLOCK_Init (void);
void PORT_Init (void);
void UART0_Init (void);
void UART1_Init (void);
INT8U KeyScan( void );
INT16U testChip(INT8U);
void drawPage(int pageNum, int chipNum);
void drawRes(unsigned int r, int n);
void Delay(void);

#endif
