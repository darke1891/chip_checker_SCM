#include "myDefine.h"
char chipName[15][10] = {"74LS00","74LS04","74LS11","74LS14","74LS20","74LS27","74LS74","74LS75","74LS85","74LS86","74LS90","74LS125","74LS161","74LS253","CD4011"};
int pinNum[15] = {14,14,14,14,14,14,14,16,16,14,14,14,16,16,14};	

void drawPage(int pageNum, int chipNum)
{
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

void drawRes(unsigned int r, int n) 
{
	int i;
	unsigned int p = pinNum[n];
	unsigned int x, upy, downy, delta;
	char num[5];
	if(p == 14)
	{
		x = 27;
		upy = 61;
		downy = 193;
		delta = 40;
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
		if(r & 0x1)   			//wrong
			printf("RED,1,1,1,\"");
		else
			printf("GREEN,1,1,1,\"");
		r = (r >> 1);
		printf("%d\"%s",i+1,c);
		if(i < p/2-1)
			x += delta;
		if(i > p/2-1)
			x -= delta;
	}
}