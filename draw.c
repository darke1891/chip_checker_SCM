#include "draw.h"
#include "myDefine.h"
/* These functions are used to draw pages on screen*/

char chipName[15][10] = {"74LS00","74LS04","74LS11","74LS14","74LS20","74LS27","74LS74","74LS75","74LS85","74LS86","74LS90","74LS125","74LS161","74LS253","CD4011"};//name of chips
int pinNum[15] = {14,14,14,14,14,14,14,16,16,14,14,14,16,16,14}; // pin number of each chip

/*this function is used to draw a page on screen
page 0:welcom page, no use
page 1:choose page, choose chip type
page 2:confirm page, confirm that right type of chip is embedded
page 3 result page, show result of test, but this function will not show reslut, just draw the chip on the screen
*/
void drawPage(int pageNum, int chipNum)
{
	UART=0;
	switch(pageNum)
	{
		case 0:
			printf("pic 0,0,0");
			printf("%s",c);
			break;
		case 1:
			if(pinNum[chipNum] == 14)
				printf("pic 0,0,2");
			else
				printf("pic 0,0,3");
			printf("%s",c);
			printf("xstr 64,0,200,32,3,BLACK,WHITE,1,1,1,\"«Î—°‘Ò–æ∆¨\"");
			printf("%s",c);
			printf("xstr 110,125,128,32,1,BLACK,WHITE,0,1,1,\"");
			printf("%s",chipName[chipNum]);
			printf("\"");
			printf("%s",c);
			break;
		case 2:
			printf("pic 0,0,1");
			printf("%s",c);
			break;
		case 3:
			if(pinNum[chipNum] == 14)
				printf("pic 0,0,2");
			else
				printf("pic 0,0,3");
			printf("%s",c);
			printf("xstr 64,0,200,32,3,BLACK,WHITE,1,1,1,\"≤‚ ‘Ω·π˚\"");
			printf("%s",c);
			printf("xstr 110,125,128,32,1,BLACK,WHITE,0,1,1,\"");
			printf("%s",chipName[chipNum]);
			printf("\"");
			printf("%s",c);
			break;
		case 4:
			printf("pic 0,0,4");
			printf("%s",c);
			break ;
		default:
			printf("pic 0,0,1");	
	}
	return;
}

/*
void itoa(unsigned int value,char* c)
{
	int i = 0;
	int st = 0,ed;
	char tmp;
	while(value != 0)
	{
		c[i] = '0' + value % 10;
		value = value / 10;	
		i ++;
	}
	ed = i-1;
	while(st < ed)
	{
		tmp = c[st];
		c[st] = c[ed];
		c[ed] = tmp;
		st ++;
		ed --;
	}
	c[i] = 0;

}
*/
//this function is used to show result of a test at page 3
void drawRes(unsigned int r, int n) 
{
	int i;
	unsigned int p = pinNum[n];
	unsigned int x, upy, downy, delta;
	//char num[5]; warning: unreferenced
	if(p == 14)
	{
		x = 27;
		upy = 61;
		downy = 193;
		delta = 40;
		// for 14_pin chip, bit 0-6, 9-15 shows result of pin 0-13
		r = ((r & (0x7F)) | (r & 0xFE00) >> 2);
	}
	else
	{
		x = 27;
		upy = 61;
		downy = 193;
		delta = 34;
	}
	for(i = 0;i < p;i ++)
	{
		printf("xstr %d,",x);
		if(i > p/2-1)	
			printf("%d",upy);
		else
			printf("%d",downy);
		if(p == 14)
			printf(",19,19,0,BLACK,");
		else
			printf(",19,19,0,BLACK,");	
		if(r & 0x1)   			// bit 1 for wrong
			printf("RED,1,1,1,\"");
		else					// bit 0 for right
			printf("GREEN,1,1,1,\"");
		r = (r >> 1);
		printf("%d\"%s",i+1,c);
		if(i < p/2-1)
			x += delta;
		if(i > p/2-1)
			x -= delta;
	}
}