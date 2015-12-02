#include "myDefine.h"

static INT8U KeyStatus = 0xFF;


INT8U KeyScan( void )
{
  INT8U btmp;

  if( KeyStatus == 0xFF )
  {
    KeyStatus = P4 & 0x1C;
  }
  if( KeyStatus == ( P4 & 0x1C ) )
  {
    return K_NONE;
  }
  btmp = KeyStatus;
  KeyStatus = P4 & 0x1C;
  btmp ^= KeyStatus;
  if( btmp & ( 1<<2 ) )
  {
    if( KeyStatus & ( 1<<2 ) )
    {
      return K1_RELEASE;
    }
    else
    {
      return K1_PRESS;
    }
  }
  if( btmp & ( 1<<3 ) )
  {
    if( KeyStatus & ( 1<<3 ) )
    {
      return K2_RELEASE;
    }
    else
    {
      return K2_PRESS;
    }
  }
  if( btmp & ( 1<<4 ) )
  {
    if( KeyStatus & ( 1<<4 ) )
    {
      return K3_RELEASE;
    }
    else
    {
      return K3_PRESS;
    }
  }
  return K_NONE;
}


INT16U testChip(INT8U num)
{
  INT16U result = 0;
  INT8U A1,B1,Y1;
  switch(num)
  {
    case 0:
      P3 = (P3 & 0xC0) | 0x35;
      P1MDOUT |= 0x1B;
      P2MDOUT |= 0x6C;
      for (A1=0;A1<2;A1++)
        for (B1=0;B1<2;B1++)
        {
          P1 = (P1 & 0xFC) | (A1 << 0) | (B1 << 1);
          Y1 = (P1 >> 2) & 0x01;
          if (Y1 == A1 & B1)
            result |= (INT16U)0x0007;
          printf("0 %d %d %d %x\n",(int)A1, (int)B1, (int)Y1, (int)result);
          P1 = (P1 & 0xE7) | (A1 << 3) | (B1 << 4);
          Y1 = (P1 >> 5) & 0x01;
          if (Y1 == A1 & B1)
            result |= (INT16U)0x0038;
          printf("1 %d %d %d %x\n",(int)A1, (int)B1, (int)Y1, (int)result);
          P2 = (P2 & 0xF3) | (A1 << 3) | (B1 << 2);
          Y1 = (P2 >> 1) & 0x01;
          if (Y1 == A1 & B1)
            result |= (INT16U)0x0E00;
          printf("2 %d %d %d %x\n",(int)A1, (int)B1, (int)Y1, (int)result);
          P2 = (P2 & 0x9F) | (A1 << 6) | (B1 << 5);
          Y1 = (P2 >> 4) & 0x01;
          if (Y1 == A1 & B1)
            result |= (INT16U)0x7000;
          printf("3 %d %d %d %x\n",(int)A1, (int)B1, (int)Y1, (int)result);
        }
      break;
    default:
      result = (INT16U)0xFEFE;
  }
//  P1MDOUT = 0x00;
//  P2MDOUT = 0x00;
  return result;
}