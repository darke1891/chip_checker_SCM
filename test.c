#include "myDefine.h"

static INT8U KeyStatus = 0x0;


INT8U KeyScan( void )
{
  INT8U btmp;

  btmp = KeyStatus;

  P4 = P4 | 0x1C;

  KeyStatus = P4 & 0x1C;

  if( KeyStatus == btmp )
  {
    return K_NONE;
  }

  btmp ^= KeyStatus;

  if ((KeyStatus == 0x10) & (btmp > 0))
    return K_CHECK;

  if( btmp & ( 1<<2 ))
  {
    if( KeyStatus & ( 1<<2 ))
    {
      return K1_RELEASE;
    }
    else
    {
      return K1_PRESS;
    }
  }
  if( btmp & ( 1<<3 ))
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

INT8U setBit8U (INT8U in, int num, INT8U b)
{
  INT8U result;
  if (num > 7)
    return -1;
  result = (INT8U)0xFF ^ ((INT8U)1 << num);
  result = (in & result) | ((b & 1) << num);
  return result;
}

INT8U getBit8U (INT8U in, int num)
{
  INT8U result;
  if (num > 7)
    return 0;
  result = in;
  result = result >> num;
  result &= 1;
  return result;
}

INT16U setBit16U (INT16U in, int num, INT16U b)
{
  INT16U result;
  if (num > 15)
    return -1;
  result = (INT16U)0xFFFF ^ ((INT16U)1 << num);
  result = (in & result) | ((b & 1) << num);
  return result;
}

INT16U getBit16U (INT16U in, int num)
{
  INT16U result;
  if (num > 15)
    return 0;
  result = in;
  result = result >> num;
  result &= 1;
  return result;
}

INT16U testChip(INT8U num)
{
  INT16U result = 0;
  switch(num)
  {
    case 0:
    case 14:
    //74LS00 and CD4011
      result = test00();
      break;
    case 1:
    case 3:
    //74LS04 and 74LS14
      result = test04();
      break;
    case 2:
    //74LS11
      result = test11();
      break;
    case 4:
    //74LS20
      result = test20();
      break;
    case 5:
    //74LS27
      result = test27();
      break;
    case 6:
    //74LS74
      result = test74();
      break;
    case 7:
    //74LS75
      result = test75();
      break;
    case 8:
    //74LS85
      result = test85();
      break;
    case 9:
    //74LS86
      result = test86();
      break;
    case 10:
    //74LS90
      result = test90();
      break;
    case 11:
    //74LS125
      result = test125();
      break;
    case 12:
    //74LS161
      result = test161();
      break;
    case 13:
    //74LS253
      result = test253();
      break;
    default:
      result = (INT16U)0xFFFF;
  }
  return result;
}

INT16U test161()
{
  INT16U result = 0;
  INT8U in, P, T, LD, R, CP, Q, C0, oldQ;
  INT8U temp8U;
//  INT16U temp16U;
  INT16U looper,i;

  temp8U = P3;
  temp8U = setBit8U(temp8U, 0, 1);
  temp8U = setBit8U(temp8U, 1, 1);
  temp8U = setBit8U(temp8U, 2, 0);
  temp8U = setBit8U(temp8U, 3, 0);
  P3 = temp8U;
  P1MDOUT = 0x7F;
  P2MDOUT = 0x3;

  for (looper = 0;looper < 0x10;looper++)
  {
    CP = getBit16U(looper, 0);
    T = getBit16U(looper, 1);
    P = getBit16U(looper, 2);
    LD = getBit16U(looper, 3);
    in = looper & 0xf;
    R = 0x0;
    set161(&in, &P, &T, &LD, &R, CP, &Q, &C0, result);
    if (Q != 0)
    {
      result = 0x7f7f;
    }
    R = 0x1;
    LD = 0x0;
    set161CP(&in, &P, &T, &LD, &R, &Q, &C0, result);
    if (Q != in)
    {
      result = 0x7f7f;
    }
    LD = 0x1;
    set161(&in, &P, &T, &LD, &R, 1, &Q, &C0, result);
    oldQ = Q;
    for (i = 0;i < 0x80;i++)
    {
      P = getBit16U(i,0);
      T = getBit16U(i,1);
      in = (i >> 2) & 0xf;
      set161CP(&in, &P, &T, &LD, &R, &Q, &C0, result);
      if ((P == 0x1) && (T == 0x1))
      {
        oldQ = (oldQ + 1) & 0xf;
      }
      if (Q != oldQ)
      {
        result = 0x7f7f;
      }
      oldQ = Q;
    }
  }

  printf("%x\n",result);
  P3 = P3 & 0xc0;
  P1MDOUT = 0x0;
  P2MDOUT = 0x0;
  return result;
}

void set161(INT8U *in, INT8U *P, INT8U *T, INT8U *LD, INT8U *R, INT8U CP, INT8U *Q, INT8U *C0, INT16U result)
{
  INT8U A,B,C,D,Q0,Q1,Q2,Q3;
  INT8U temp8U;
  A = getBit8U(*in, 0);
  B = getBit8U(*in, 1);
  C = getBit8U(*in, 2);
  D = getBit8U(*in, 3);
  temp8U = P1;
//  nR = (*R) ^ 0x1;
  temp8U = setBit8U(temp8U, 0, *R);
  temp8U = setBit8U(temp8U, 1, CP);
  temp8U = setBit8U(temp8U, 2, A);
  temp8U = setBit8U(temp8U, 3, B);
  temp8U = setBit8U(temp8U, 4, C);
  temp8U = setBit8U(temp8U, 5, D);
  temp8U = setBit8U(temp8U, 6, *P);
  P1 = temp8U;
  temp8U = P2;
  temp8U = setBit8U(temp8U, 0, *LD);
  temp8U = setBit8U(temp8U, 1, *T);
  P2 = temp8U;

  printf("%x ",result);
  printf("%x ",result);
  printf("%x ",result);
  printf("%x\n",result);

  P2 = P2 | 0x7c;
  temp8U = P2;
  *C0 = getBit8U(temp8U, 6);
  Q0 = getBit8U(temp8U, 5);
  Q1 = getBit8U(temp8U, 4);
  Q2 = getBit8U(temp8U, 3);
  Q3 = getBit8U(temp8U, 2);
  *Q = Q0 + (Q1 << 1) + (Q2 << 2) + (Q3 << 3);
}

void set161CP(INT8U *in, INT8U *P, INT8U *T, INT8U *LD, INT8U *R, INT8U *Q, INT8U *C0, INT16U result)
{
  set161(in, P, T, LD, R, 0, Q, C0, result);
  set161(in, P, T, LD, R, 1, Q, C0, result);
}

INT16U test90()
{
  INT16U result = 0;
  INT8U R1,R2,S1,S2,Q0,oldQ0,oldQ13,Q13;
  INT8U temp8U;
//  INT16U temp16U;
  INT16U looper,i;

  temp8U = P3;
  temp8U = setBit8U(temp8U, 0, 1);
  temp8U = setBit8U(temp8U, 1, 0);
  temp8U = setBit8U(temp8U, 2, 0);
  temp8U = setBit8U(temp8U, 3, 1);
  P3 = temp8U;
  P1MDOUT = 0x67;
  P2MDOUT = 0x80;

  for (looper = 0; looper < 0x10; looper++)
  {
    temp8U = getBit16U(looper, 0);
    if (temp8U == 0)
    {
      R1 = 0x1;
      R2 = 0x1;
      S1 = getBit16U(looper, 1);
      if (S1 == 0x1)
        S2 = 0x0;
      else
        S2 = getBit16U(looper, 2);
    }
    else
    {
      R1 = getBit16U(looper, 1);
      R2 = getBit16U(looper, 2);
      S1 = 0x1;
      S2 = 0x1;
    }
    set90(&R1, &R2, &S1, &S2, &Q0, &Q13, 0, 0, result);
    if (temp8U == 0)
    {
      if ((Q0 != 0) | (Q13 != 0))
      {
        result |= 0x3666;
      }
    }
    else
      if ((Q0 != 1) | (Q13 != 0x4))
      {
        result |= 0x3666;
      }
    if (temp8U == 0)
    {
      R1 = getBit16U(looper, 1);
      R2 = 0x0;
      S1 = getBit16U(looper, 2);
      S2 = 0x0;
    }
    else
    {
      R2 = getBit16U(looper, 1);
      R1 = 0x0;
      S2 = getBit16U(looper, 2);
      S1 = 0x0;
    }
    set90(&R1, &R2, &S1, &S2, &Q0, &Q13, 0, 0, result);
    oldQ0 = Q0;
    oldQ13 = Q13;
    for (i = 0; i < 20; i++)
    {
      if (i % 2 == 0)
      {
        set90B(&R1, &R2, &S1, &S2, &Q0, &Q13, result);
        if (Q13 != (oldQ13 + 1) % 5)
        {
          result = 0xb667;
        }
        oldQ13 = Q13;
      }
      else
      {
        set90A(&R1, &R2, &S1, &S2, &Q0, &Q13, result);
        if (Q0 != (oldQ0 + 1) % 2)
        {
          result = 0xb667;
        }
        oldQ0 = Q0;
      }
    }
  }
  P3 = P3 & 0xc0;
  P1MDOUT = 0x0;
  P2MDOUT = 0x0;
  return result;
}

void set90A(INT8U *R1, INT8U *R2, INT8U *S1, INT8U *S2,
            INT8U *Q0, INT8U *Q13,
            INT16U result)
{
  set90 (R1, R2, S1, S2, Q0, Q13, 1, 0, result);
  set90 (R1, R2, S1, S2, Q0, Q13, 0, 0, result);
}

void set90B(INT8U *R1, INT8U *R2, INT8U *S1, INT8U *S2,
            INT8U *Q0, INT8U *Q13,
            INT16U result)
{
  set90 (R1, R2, S1, S2, Q0, Q13, 0, 1, result);
  set90 (R1, R2, S1, S2, Q0, Q13, 0, 0, result);
}

void set90(INT8U *R1, INT8U *R2, INT8U *S1, INT8U *S2,
           INT8U *Q0, INT8U *Q13,
           INT8U nCPA, INT8U nCPB, INT16U result)
{
  INT8U temp8U;
  INT8U Q1,Q2,Q3;
  temp8U = P1;
  temp8U = setBit8U(temp8U, 0, nCPB);
  temp8U = setBit8U(temp8U, 1, *R1);
  temp8U = setBit8U(temp8U, 2, *R2);
  temp8U = setBit8U(temp8U, 5, *S1);
  temp8U = setBit8U(temp8U, 6, *S2);
  P1 = temp8U;
  temp8U = P2;
  temp8U = setBit8U(temp8U, 7, nCPA);
  P2 = temp8U;

  printf("%x ",result);
  printf("%x ",result);
  printf("%x ",result);
  printf("%x\n",result);

  P2 = P2 | 0x36;
  temp8U = P2;
  *Q0 = getBit8U(temp8U, 5);
  Q1 = getBit8U(temp8U, 2);
  Q2 = getBit8U(temp8U, 1);
  Q3 = getBit8U(temp8U, 4);
  *Q13 = Q1 + (Q2 << 1) + (Q3 << 2);
}

INT16U test253()
{
  INT16U result = 0;
  INT8U nEN,A0,A1,D0,D1,D2,D3,Y,id;
  INT8U temp8U;
//  INT16U temp16U;
  INT16U looper;
  P1MDOUT = 0x3f;
  P2MDOUT = 0x7e;
  temp8U = P3;
  temp8U = setBit8U(temp8U, 0, 1);
  temp8U = setBit8U(temp8U, 1, 1);
  temp8U = setBit8U(temp8U, 2, 0);
  temp8U = setBit8U(temp8U, 3, 0);
  P3 = temp8U;
  nEN = 0x0;
  for (id = 0; id < 0x2; id++)
    for (looper = 0; looper < 0x80; looper++)
    {
      A0 = getBit16U(looper, 0);
      A1 = getBit16U(looper, 1);
      D0 = getBit16U(looper, 2);
      D1 = getBit16U(looper, 3);
      D2 = getBit16U(looper, 4);
      D3 = getBit16U(looper, 5);
      set253(&A0, &A1, &D0, &D1, &D2, &D3, &nEN, &Y, id, result);
      if (A1 == 0x0)
      {
        if (A0 == 0x0)
        {
          if (D0 != Y)
            fault253(id, &result);
        }
        else
          if (D1 != Y)
            fault253(id, &result);
      }
      else
      {
        if (A0 == 0x0)
        {
          if (D2 != Y)
            fault253(id, &result);
        }
        else
          if (D3 != Y)
            fault253(id, &result);
      }
    }
  P1MDOUT = 0x0;
  P2MDOUT = 0x0;
  P3 = P3 & 0xc0;
  return result;
}

void set253(INT8U *A0, INT8U *A1, INT8U *D0, INT8U *D1, INT8U *D2, INT8U *D3, INT8U *nEN, INT8U *Y, INT8U id, INT16U result)
{
  INT8U temp8U;
  if (id == 0)
  {
    temp8U = P1;
    temp8U = setBit8U(temp8U, 0, *nEN);
    temp8U = setBit8U(temp8U, 1, *A1);
    temp8U = setBit8U(temp8U, 2, *D3);
    temp8U = setBit8U(temp8U, 3, *D2);
    temp8U = setBit8U(temp8U, 4, *D1);
    temp8U = setBit8U(temp8U, 5, *D0);
    P1 = temp8U;
    temp8U = P2;
    temp8U = setBit8U(temp8U, 5, *A0);
    P2 = temp8U;
    printf("%x ",result);
    printf("%x ",result);
    printf("%x ",result);
    printf("%x\n",result);
    P1 = P1 | 0x40;
    temp8U = P1;
    *Y = getBit8U(temp8U, 6);
  }
  else
  {
    temp8U = P1;
    temp8U = setBit8U(temp8U, 1, *A1);
    P1 = temp8U;
    temp8U = P2;
    temp8U = setBit8U(temp8U, 6, *nEN);
    temp8U = setBit8U(temp8U, 5, *A0);
    temp8U = setBit8U(temp8U, 4, *D3);
    temp8U = setBit8U(temp8U, 3, *D2);
    temp8U = setBit8U(temp8U, 2, *D1);
    temp8U = setBit8U(temp8U, 1, *D0);
    P2 = temp8U;
    printf("%x ",result);
    printf("%x ",result);
    printf("%x ",result);
    printf("%x\n",result);
    P2 = P2 | 0x1;
    temp8U = P2;
    *Y = getBit8U(temp8U, 0);
  }
}

void fault253(INT8U id, INT16U *result)
{
  if (id == 0)
  {
    *result = *result | (INT16U)0x207f;
  }
  else
  {
    *result = *result | (INT16U)0x7f02;
  }
}

INT16U test85()
{
  INT16U result = 0;
  INT8U iP0,iQ0,iP1,iQ1,iP2,iQ2,iP3,iQ3,iPg,iPs,iPe,outG,outS,outE;
  INT8U temp8U;
//  INT16U temp16U;
  INT16U looper;
  P1MDOUT = 0x0f;
  P2MDOUT = 0x7f;
  temp8U = P3;
  temp8U = setBit8U(temp8U, 0, 1);
  temp8U = setBit8U(temp8U, 1, 1);
  temp8U = setBit8U(temp8U, 2, 0);
  temp8U = setBit8U(temp8U, 3, 0);
  P3 = temp8U;
  for (looper = 0;looper < (INT16U)0x800;looper++)
  {
    iP0 = getBit16U(looper, 0);
    iQ0 = getBit16U(looper, 1);
    iP1 = getBit16U(looper, 2);
    iQ1 = getBit16U(looper, 3);
    iP2 = getBit16U(looper, 4);
    iQ2 = getBit16U(looper, 5);
    iP3 = getBit16U(looper, 6);
    iQ3 = getBit16U(looper, 7);
    iPg = getBit16U(looper, 8);
    iPs = getBit16U(looper, 9);
    iPe = getBit16U(looper, 10);

    temp8U = P1;
    temp8U = setBit8U(temp8U, 0, iQ3);
    temp8U = setBit8U(temp8U, 1, iPs);
    temp8U = setBit8U(temp8U, 2, iPe);
    temp8U = setBit8U(temp8U, 3, iPg);
    P1 = temp8U;
    temp8U = P2;
    temp8U = setBit8U(temp8U, 0, iQ0);
    temp8U = setBit8U(temp8U, 1, iP0);
    temp8U = setBit8U(temp8U, 2, iQ1);
    temp8U = setBit8U(temp8U, 3, iP1);
    temp8U = setBit8U(temp8U, 4, iP2);
    temp8U = setBit8U(temp8U, 5, iQ2);
    temp8U = setBit8U(temp8U, 6, iP3);
    P2 = temp8U;

    printf("%x ",result);
    printf("%x ",result);
    printf("%x ",result);
    printf("%x\n",result);

    P1 = (P1 | 0x70);
    temp8U = P1;
    outS = getBit8U(temp8U, 6);
    outE = getBit8U(temp8U, 5);
    outG = getBit8U(temp8U, 4);
    if ((iP3 > iQ3) || 
        ((iP3 == iQ3) && (iP2 > iQ2)) ||
        ((iP3 == iQ3) && (iP2 == iQ2) && (iP1 > iQ1)) ||
        ((iP3 == iQ3) && (iP2 == iQ2) && (iP1 == iQ1) && (iP0 > iQ0)) ||
        ((iP3 == iQ3) && (iP2 == iQ2) && (iP1 == iQ1) && (iP0 == iQ0) && (iPg == 1) && (iPe == 0) && (iPs == 0)))
    {
      if ((outG == 0) || (outE == 1) || (outS == 1))
        result = (INT16U)0x7f7f;
    }
    else if ((iP3 < iQ3) ||
             ((iP3 == iQ3) && (iP2 < iQ2)) ||
             ((iP3 == iQ3) && (iP2 == iQ2) && (iP1 <iQ1)) ||
             ((iP3 == iQ3) && (iP2 == iQ2) && (iP1 == iQ1) && (iP0 < iQ0)) ||
             ((iP3 == iQ3) && (iP2 == iQ2) && (iP1 == iQ1) && (iP0 == iQ0) && (iPg == 0) && (iPe == 0) && (iPs == 1))) 
    {
      if ((outG == 1) || (outE == 1) || (outS == 0))
        result = (INT16U)0x7f7f;
    }
    else if ((iP3 == iQ3) && (iP2 == iQ2) && (iP1 == iQ1) && (iP0 == iQ0) && (iPe == 1))
    {
      if ((outG == 1) || (outE == 0) || (outS == 1))
        result = (INT16U)0x7f7f;
    }
    else if ((iP3 == iQ3) && (iP2 == iQ2) && (iP1 == iQ1) && (iP0 == iQ0))
    {
      if ((iPg == 1) && (iPs == 1))
      {
        if ((outE == 1) || (outS == 1) || (outG == 1))
          result = (INT16U)0x7f7f;
      }
      else if ((iPg == 0) && (iPs == 0))
      {
        if ((outE == 1) || (outS == 0) || (outG == 0))
          result = (INT16U)0x7f7f;
      }
      else
        result = (INT16U)0x7f7f;
    }
    else
      result = (INT16U)0x7f7f;
  }
  P1MDOUT = 0x0;
  P2MDOUT = 0x0;
  P3 = P3 & 0xc0;
  return result;
}

INT16U test75()
{
  INT16U result = 0;
  INT8U CP,D1,Q1,nQ1,oldQ1,oldnQ1,D2,Q2,nQ2,oldQ2,oldnQ2;
  INT8U temp8U;
  INT16U temp16U;
  INT16U i,j,id;
  temp8U = P3;
  temp8U = setBit8U(temp8U, 0, 1);
  temp8U = setBit8U(temp8U, 1, 0);
  temp8U = setBit8U(temp8U, 2, 0);
  temp8U = setBit8U(temp8U, 3, 1);
  P3 = temp8U;
  P1MDOUT = 0x6e;
  P2MDOUT = 0x10;
  for (id = 0x0;id<0x2;id++)
  {
    for (i =0;i<0x04;i++)
    {
      D1 = getBit8U((INT8U)i, 0);
      D2 = getBit8U((INT8U)i, 1);
      CP = (INT8U)0x01;
      set75(&CP, &D1, &Q1, &nQ1, &D2, &Q2, &nQ2, id, result);
      if ((Q1 != D1) || (nQ1 == D1))
        fault75(id*2, &result);
      if ((Q2 != D2) || (nQ2 == D2))
        fault75(id*2+1, &result);
      oldQ1 = Q1;
      oldnQ1 = nQ1;
      oldQ2 = Q2;
      oldnQ2 = nQ2;
      temp16U = D1;
      CP = (INT8U)0x0;
      set75(&CP, &D1, &Q1, &nQ1, &D2, &Q2, &nQ2, id, result);
      for (j = 0;j<0x04;j++)
      {
        D1 = getBit8U((INT8U)j, 0);
        D2 = getBit8U((INT8U)j, 1);
        set75(&CP, &D1, &Q1, &nQ1, &D2, &Q2, &nQ2, id, result);
        if ((Q1 != oldQ1) || (nQ1 != oldnQ1))
          fault75(id*2, &result);
        if ((Q2 != oldQ2) || (nQ2 != oldnQ2))
          fault75(id*2+1, &result);
      }
    }
  }
  P1MDOUT = 0x0;
  P2MDOUT = 0x0;
  P3 = P3 & (INT8U)0xC0;
  return result;
}
void set75( INT8U *CP,
            INT8U *D1, INT8U *Q1, INT8U *nQ1,
            INT8U *D2, INT8U *Q2, INT8U *nQ2,
            INT16U id, INT16U result)
{
  INT8U temp8U;
  if (id == 0)
  {
    temp8U = P1;
    temp8U = setBit8U(temp8U, 1, *D1);
    temp8U = setBit8U(temp8U, 2, *D2);
    P1 = temp8U;
    temp8U = P2;
    temp8U = setBit8U(temp8U, 4, *CP);
    P2 = temp8U;
    printf("%x ",result);
    printf("%x ",result);
    printf("%x ",result);
    printf("%x\n",result);
    P1 = (P1 | 0x1);
    temp8U = P1;
    *nQ1 = getBit8U(temp8U, 0);
    P2 = (P2 | 0xe0);
    temp8U = P2;
    *Q1 = getBit8U(temp8U, 7);
    *Q2 = getBit8U(temp8U, 6);
    *nQ2 = getBit8U(temp8U, 5);
  }
  else
  {
    temp8U = P1;
    temp8U = setBit8U(temp8U, 5, *D1);
    temp8U = setBit8U(temp8U, 6, *D2);
    temp8U = setBit8U(temp8U, 3, *CP);
    P1 = temp8U;
    printf("%x ",result);
    printf("%x ",result);
    printf("%x ",result);
    printf("%x\n",result);
    P1 = (P1 | 0x80);
    temp8U = P1;
    *nQ2 = getBit8U(temp8U, 7);
    P2 = (P2 | 0x7);
    temp8U = P2;
    *nQ1 = getBit8U(temp8U, 2);
    *Q1 = getBit8U(temp8U, 1);
    *Q2 = getBit8U(temp8U, 0);
  }
}

void fault75(INT16U id, INT16U *result)
{
  if (id == 0)
    *result = *result | (INT16U)0x9003;
  else if (id == 1)
    *result = *result | (INT16U)0x7004;
  else if (id == 2)
    *result = *result | (INT16U)0x0628;
  else
    *result = *result | (INT16U)0x01c8;
}

INT16U test74()
{
  INT16U result = 0;
  INT8U nS,nR,CP,D,Q,nQ,oldD;
  INT8U temp8U;
  INT16U i,j,id;
//  INT16U temp16U;
  P1MDOUT = 0x0F;
  P2MDOUT = 0x78;  
  temp8U = P3;
  temp8U = setBit8U(temp8U, 0, 1);
  temp8U = setBit8U(temp8U, 1, 0);
  temp8U = setBit8U(temp8U, 2, 1);
  temp8U = setBit8U(temp8U, 3, 0);
  temp8U = setBit8U(temp8U, 5, 1);
  P3 = temp8U;
  printf("\n");

  for (id = 0;id < 0x02;id++)
  {
    for (i = 0;i < 0x04;i++)
    {
      nS = i & (INT8U)1;
      nR = (i & (INT8U)1) ^ ((INT8U)0x1);
      for (j = 0;j < 0x08;j++)
      {
        CP = getBit8U(j, 0);
        D = getBit8U(j, 1);
        set74(&nS, &nR, &CP, &D, &Q, &nQ, id, result);
        if ((Q == nS) || (nQ != nS))
          fault74(id, &result);
      }
    }
    nS = (INT8U)0x01;
    nR = (INT8U)0x01;
    for (i = 0;i < 0x04;i++)
    {
      D = i & (INT8U)1;
      oldD = D;
      CP = (INT8U)0x00;
      set74(&nS, &nR, &CP, &D, &Q, &nQ, id, result);
      CP = (INT8U)0x01;
      set74(&nS, &nR, &CP, &D, &Q, &nQ, id, result);
      if ((Q != D) || (nQ == D))
        fault74(id,&result);
      for (j=0;j<0x04;j++)
      {
        D = j & (INT8U)1;
        set74(&nS, &nR, &CP, &D, &Q, &nQ, id, result);
        if ((Q != oldD) || (nQ == oldD))
          fault74(id, &result);
      }
      CP = (INT8U)0x00;
      set74(&nS, &nR, &CP, &D, &Q, &nQ, id, result);
      if ((Q != oldD) || (nQ == oldD))
        fault74(id, &result);
      for (j=0;j<0x04;j++)
      {
        D = j & (INT8U)1;
        set74(&nS, &nR, &CP, &D, &Q, &nQ, id, result);
        if ((Q != oldD) || (nQ == oldD))
          fault74(id, &result);
      }
    }
  }

  P1MDOUT = 0x00;
  P2MDOUT = 0x00;
  P3 = P3 & 0xC0;
  return result; 
}

void fault74(INT8U id, INT16U *result)
{
  if (id == 0)
    *result = *result | (INT16U)0x3f;
  else
    *result = *result | (INT16U)0x7e00;
}

void set74(INT8U *nS, INT8U *nR, INT8U *CP, INT8U *D, INT8U *Q, INT8U *nQ, INT8U id, INT16U result)
{
  INT8U temp8U;
  if (id == 0)
  {
    temp8U = P1;
    temp8U = setBit8U(temp8U, 0, *nR);
    temp8U = setBit8U(temp8U, 1, *D);
    temp8U = setBit8U(temp8U, 2, *CP);
    temp8U = setBit8U(temp8U, 3, *nS);
    P1 = temp8U;
    printf("%x\n",result);
    P1 = (P1 | 0x30);
    temp8U = P1;
    *Q = getBit8U(temp8U, 4);
    *nQ = getBit8U(temp8U, 5);
  }
  else
  {
    temp8U = P2;
    temp8U = setBit8U(temp8U, 6, *nR);
    temp8U = setBit8U(temp8U, 5, *D);
    temp8U = setBit8U(temp8U, 4, *CP);
    temp8U = setBit8U(temp8U, 3, *nS);
    P2 = temp8U;
    printf("%x\n",result);
    P2 = (P2 | 0x6);
    temp8U = P2;
    *Q = getBit8U(temp8U, 2);
    *nQ = getBit8U(temp8U, 1);
  }
}

INT16U test125()
{
  INT16U result = 0;
  INT8U A1,nEN1,Y1,A2,nEN2,Y2,A3,nEN3,Y3,A4,nEN4,Y4;
  INT8U temp8U;
//  INT16U temp16U;
  INT16U looper;
  P1MDOUT = 0x1B;
  P2MDOUT = 0x6C;
  temp8U = P3;
  temp8U = setBit8U(temp8U, 0, 1);
  temp8U = setBit8U(temp8U, 1, 0);
  temp8U = setBit8U(temp8U, 2, 1);
  temp8U = setBit8U(temp8U, 3, 0);
  temp8U = setBit8U(temp8U, 5, 1);
  P3 = temp8U;
  printf("\n");
  for (looper = 0;looper < 0x10;looper++)
  {
    A1 = getBit16U(looper, 0);
    nEN1 = 0;
    A2 = getBit16U(looper, 1);
    nEN2 = 0;
    A3 = getBit16U(looper, 2);
    nEN3 = 0;
    A4 = getBit16U(looper, 3);
    nEN4 = 0;

    temp8U = P1;
    temp8U = setBit8U(temp8U, 0, nEN1);
    temp8U = setBit8U(temp8U, 1, A1);
    temp8U = setBit8U(temp8U, 3, nEN2);
    temp8U = setBit8U(temp8U, 4, A2);
    P1 = temp8U;
    temp8U = P2;
    temp8U = setBit8U(temp8U, 3, nEN3);
    temp8U = setBit8U(temp8U, 2, A3);
    temp8U = setBit8U(temp8U, 6, nEN4);
    temp8U = setBit8U(temp8U, 5, A4);
    P2 = temp8U;

    printf("%x\n",result);
    P1 = (P1 | 0x24);
    temp8U = P1;
    Y1 = getBit8U(temp8U, 2);
    Y2 = getBit8U(temp8U, 5);
    P2 = (P2 | 0x12);
    temp8U = P2;
    Y3 = getBit8U(temp8U, 1);
    Y4 = getBit8U(temp8U, 4);

    if (Y1 != A1)
    {
      result = setBit16U(result,0,1);
      result = setBit16U(result,1,1);
      result = setBit16U(result,2,1);
    }
    if (Y2 != A2)
    {
      result = setBit16U(result,3,1);
      result = setBit16U(result,4,1);
      result = setBit16U(result,5,1);
    }
    if (Y3 != A3)
    {
      result = setBit16U(result,9,1);
      result = setBit16U(result,10,1);
      result = setBit16U(result,11,1);
    }
    if (Y4 != A4)
    {
      result = setBit16U(result,12,1);
      result = setBit16U(result,13,1);
      result = setBit16U(result,14,1);
    }
  }
  P3 = P3 & 0xC0;
  P1MDOUT = 0;
  P2MDOUT = 0;
  return result;
}

INT16U test86()
{
  INT16U result = 0;
  INT8U A1,B1,Y1,A2,B2,Y2,A3,B3,Y3,A4,B4,Y4;
  INT8U temp8U;
//  INT16U temp16U;
  INT16U looper;
  P1MDOUT = 0x1B;
  P2MDOUT = 0x6C;
  temp8U = P3;
  temp8U = setBit8U(temp8U, 0, 1);
  temp8U = setBit8U(temp8U, 1, 0);
  temp8U = setBit8U(temp8U, 2, 1);
  temp8U = setBit8U(temp8U, 3, 0);
  temp8U = setBit8U(temp8U, 5, 1);
  P3 = temp8U;
  printf("\n");
  for (looper = 0;looper < 0x100;looper++)
  {
    A1 = getBit16U(looper, 0);
    B1 = getBit16U(looper, 1);
    A2 = getBit16U(looper, 2);
    B2 = getBit16U(looper, 3);
    A3 = getBit16U(looper, 4);
    B3 = getBit16U(looper, 5);
    A4 = getBit16U(looper, 6);
    B4 = getBit16U(looper, 7);

    temp8U = P1;
    temp8U = setBit8U(temp8U, 0, A1);
    temp8U = setBit8U(temp8U, 1, B1);
    temp8U = setBit8U(temp8U, 3, A2);
    temp8U = setBit8U(temp8U, 4, B2);
    P1 = temp8U;
    temp8U = P2;
    temp8U = setBit8U(temp8U, 3, A3);
    temp8U = setBit8U(temp8U, 2, B3);
    temp8U = setBit8U(temp8U, 6, A4);
    temp8U = setBit8U(temp8U, 5, B4);
    P2 = temp8U;

    printf("%x\n",result);
    P1 = (P1 | 0x24);
    temp8U = P1;
    Y1 = getBit8U(temp8U, 2);
    Y2 = getBit8U(temp8U, 5);
    P2 = (P2 | 0x12);
    temp8U = P2;
    Y3 = getBit8U(temp8U, 1);
    Y4 = getBit8U(temp8U, 4);

    if (Y1 != (A1 ^ B1))
    {
      result = setBit16U(result,0,1);
      result = setBit16U(result,1,1);
      result = setBit16U(result,2,1);
    }
    if (Y2 != (A2 ^ B2))
    {
      result = setBit16U(result,3,1);
      result = setBit16U(result,4,1);
      result = setBit16U(result,5,1);
    }
    if (Y3 != (A3 ^ B3))
    {
      result = setBit16U(result,9,1);
      result = setBit16U(result,10,1);
      result = setBit16U(result,11,1);
    }
    if (Y4 != (A4 ^ B4))
    {
      result = setBit16U(result,12,1);
      result = setBit16U(result,13,1);
      result = setBit16U(result,14,1);
    }
  }
  P3 = P3 & 0xC0;
  P1MDOUT = 0;
  P2MDOUT = 0;
  return result;
}

INT16U test27()
{
  INT16U result = 0;
  INT8U A1,B1,C1,Y1,A2,B2,C2,Y2,A3,B3,C3,Y3;
  INT8U temp8U;
//  INT16U temp16U;
  INT16U looper;
  P1MDOUT = 0x1F;
  P2MDOUT = 0x5C;
  temp8U = P3;
  temp8U = setBit8U(temp8U, 0, 1);
  temp8U = setBit8U(temp8U, 1, 0);
  temp8U = setBit8U(temp8U, 2, 1);
  temp8U = setBit8U(temp8U, 3, 0);
  temp8U = setBit8U(temp8U, 5, 1);
  P3 = temp8U;
  printf("\n");
  for (looper = 0;looper < 0x200;looper++)
  {
    A1 = getBit16U(looper, 0);
    B1 = getBit16U(looper, 1);
    C1 = getBit16U(looper, 2);
    A2 = getBit16U(looper, 3);
    B2 = getBit16U(looper, 4);
    C2 = getBit16U(looper, 5);
    A3 = getBit16U(looper, 6);
    B3 = getBit16U(looper, 7);
    C3 = getBit16U(looper, 8);

    temp8U = P1;
    temp8U = setBit8U(temp8U, 0, A1);
    temp8U = setBit8U(temp8U, 1, B1);
    temp8U = setBit8U(temp8U, 2, A2);
    temp8U = setBit8U(temp8U, 3, B2);
    temp8U = setBit8U(temp8U, 4, C2);
    P1 = temp8U;
    temp8U = P2;
    temp8U = setBit8U(temp8U, 6, C1);
    temp8U = setBit8U(temp8U, 4, C3);
    temp8U = setBit8U(temp8U, 3, B3);
    temp8U = setBit8U(temp8U, 2, A3);
    P2 = temp8U;

    printf("%x\n",result);
    P1 = (P1 | 0x20);
    temp8U = P1;
    Y2 = getBit8U(temp8U, 5);
    P2 = (P2 | 0x22);
    temp8U = P2;
    Y1 = getBit8U(temp8U, 5);
    Y3 = getBit8U(temp8U, 1);

    if (Y1 == (A1 | B1 | C1))
    {
      result = setBit16U(result, 0, 1);
      result = setBit16U(result, 1, 1);
      result = setBit16U(result, 13, 1);
      result = setBit16U(result, 14, 1);
    }
    if (Y2 == (A2 | B2 | C2))
    {
      result = setBit16U(result, 2, 1);
      result = setBit16U(result, 3, 1);
      result = setBit16U(result, 4, 1);
      result = setBit16U(result, 5, 1);
    }
    if (Y3 == (A3 | B3 | C3))
    {
      result = setBit16U(result, 9, 1);
      result = setBit16U(result, 10, 1);
      result = setBit16U(result, 11, 1);
      result = setBit16U(result, 12, 1);
    }
  }
  P3 = P3 & 0xC0;
  P1MDOUT = 0;
  P2MDOUT = 0;
  return result;
}

INT16U test20()
{
  INT16U result = 0;
  INT8U A1,B1,C1,D1,N1,Y1,A2,B2,C2,D2,N2,Y2;
  INT8U temp8U;
//  INT16U temp16U;
  INT16U looper;
  P1MDOUT = 0x1F;
  P2MDOUT = 0x5C;
  temp8U = P3;
  temp8U = setBit8U(temp8U, 0, 1);
  temp8U = setBit8U(temp8U, 1, 0);
  temp8U = setBit8U(temp8U, 2, 1);
  temp8U = setBit8U(temp8U, 3, 0);
  temp8U = setBit8U(temp8U, 5, 1);
  P3 = temp8U;
  printf("\n");
  for (looper = 0;looper < 0x400;looper++)
  {
    A1 = getBit16U(looper, 0);
    B1 = getBit16U(looper, 1);
    C1 = getBit16U(looper, 2);
    D1 = getBit16U(looper, 3);
    N1 = getBit16U(looper, 4);
    A2 = getBit16U(looper, 5);
    B2 = getBit16U(looper, 6);
    C2 = getBit16U(looper, 7);
    D2 = getBit16U(looper, 8);
    N2 = getBit16U(looper, 9);


    temp8U = P1;
    temp8U = setBit8U(temp8U, 0, A1);
    temp8U = setBit8U(temp8U, 1, B1);
    temp8U = setBit8U(temp8U, 2, N1);
    temp8U = setBit8U(temp8U, 3, C1);
    temp8U = setBit8U(temp8U, 4, D1);
    P1 = temp8U;
    temp8U = P2;
    temp8U = setBit8U(temp8U, 6, A2);
    temp8U = setBit8U(temp8U, 5, B2);
    temp8U = setBit8U(temp8U, 4, N2);
    temp8U = setBit8U(temp8U, 3, C2);
    temp8U = setBit8U(temp8U, 2, D2);
    P2 = temp8U;

    printf("%x\n",result);
    P1 = (P1 | 0x20);
    temp8U = P1;
    Y1 = getBit8U(temp8U, 5);
    P2 = (P2 | 0x2);
    temp8U = P2;
    Y2 = getBit8U(temp8U, 1);


    if (Y1 == (A1 & B1 & C1 & D1))
    {
      result = setBit16U(result, 0, 1);
      result = setBit16U(result, 1, 1);
      result = setBit16U(result, 3, 1);
      result = setBit16U(result, 4, 1);
      result = setBit16U(result, 5, 1);
    }
    if (Y2 == (A2 & B2 & C2 & D2))
    {
      result = setBit16U(result, 14, 1);
      result = setBit16U(result, 13, 1);
      result = setBit16U(result, 11, 1);
      result = setBit16U(result, 10, 1);
      result = setBit16U(result, 9, 1);
    }
  }
  P3 = P3 & 0xC0;
  P1MDOUT = 0;
  P2MDOUT = 0;
  return result;
}

INT16U test11()
{
  INT16U result = 0;
  INT8U A1,B1,C1,Y1,A2,B2,C2,Y2,A3,B3,C3,Y3;
  INT8U temp8U;
//  INT16U temp16U;
  INT16U looper;
  P1MDOUT = 0x1F;
  P2MDOUT = 0x5C;
  temp8U = P3;
  temp8U = setBit8U(temp8U, 0, 1);
  temp8U = setBit8U(temp8U, 1, 0);
  temp8U = setBit8U(temp8U, 2, 1);
  temp8U = setBit8U(temp8U, 3, 0);
  temp8U = setBit8U(temp8U, 5, 1);
  P3 = temp8U;
  printf("\n");
  for (looper = 0;looper < 0x200;looper++)
  {
    A1 = getBit16U(looper, 0);
    B1 = getBit16U(looper, 1);
    C1 = getBit16U(looper, 2);
    A2 = getBit16U(looper, 3);
    B2 = getBit16U(looper, 4);
    C2 = getBit16U(looper, 5);
    A3 = getBit16U(looper, 6);
    B3 = getBit16U(looper, 7);
    C3 = getBit16U(looper, 8);

    temp8U = P1;
    temp8U = setBit8U(temp8U, 0, A1);
    temp8U = setBit8U(temp8U, 1, B1);
    temp8U = setBit8U(temp8U, 2, A2);
    temp8U = setBit8U(temp8U, 3, B2);
    temp8U = setBit8U(temp8U, 4, C2);
    P1 = temp8U;
    temp8U = P2;
    temp8U = setBit8U(temp8U, 6, C1);
    temp8U = setBit8U(temp8U, 4, C3);
    temp8U = setBit8U(temp8U, 3, B3);
    temp8U = setBit8U(temp8U, 2, A3);
    P2 = temp8U;

    printf("%x\n",result);
    P1 = (P1 | 0x2);
    temp8U = P1;
    Y2 = getBit8U(temp8U, 5);
    P2 = (P2 | 0x22);
    temp8U = P2;
    Y1 = getBit8U(temp8U, 5);
    Y3 = getBit8U(temp8U, 1);


    if (Y1 != (A1 & B1 & C1))
    {
      result = setBit16U(result, 0, 1);
      result = setBit16U(result, 1, 1);
      result = setBit16U(result, 13, 1);
      result = setBit16U(result, 14, 1);
    }
    if (Y2 != (A2 & B2 & C2))
    {
      result = setBit16U(result, 2, 1);
      result = setBit16U(result, 3, 1);
      result = setBit16U(result, 4, 1);
      result = setBit16U(result, 5, 1);
    }
    if (Y3 != (A3 & B3 & C3))
    {
      result = setBit16U(result, 9, 1);
      result = setBit16U(result, 10, 1);
      result = setBit16U(result, 11, 1);
      result = setBit16U(result, 12, 1);
    }
  }
  P3 = P3 & 0xC0;
  P1MDOUT = 0;
  P2MDOUT = 0;
  return result;
}

INT16U test04()
{
  INT16U result = 0;
  INT8U A1,Y1,A2,Y2,A3,Y3,A4,Y4,A5,Y5,A6,Y6;
  INT8U temp8U;
//  INT16U temp16U;
  INT16U looper;
  P1MDOUT = 0x15;
  P2MDOUT = 0x54;
  temp8U = P3;
  temp8U = setBit8U(temp8U, 0, 1);
  temp8U = setBit8U(temp8U, 1, 0);
  temp8U = setBit8U(temp8U, 2, 1);
  temp8U = setBit8U(temp8U, 3, 0);
  temp8U = setBit8U(temp8U, 5, 1);
  P3 = temp8U;
  printf("\n");
  for (looper = 0;looper < 0x40;looper++)
  {
    A1 = getBit16U(looper, 0);
    A2 = getBit16U(looper, 1);
    A3 = getBit16U(looper, 2);
    A4 = getBit16U(looper, 3);
    A5 = getBit16U(looper, 4);
    A6 = getBit16U(looper, 5);

    temp8U = P1;
    temp8U = setBit8U(temp8U, 0, A1);
    temp8U = setBit8U(temp8U, 2, A2);
    temp8U = setBit8U(temp8U, 4, A3);
    P1 = temp8U;
    temp8U = P2;
    temp8U = setBit8U(temp8U, 2, A4);
    temp8U = setBit8U(temp8U, 4, A5);
    temp8U = setBit8U(temp8U, 6, A6);
    P2 = temp8U;

    printf("%x \n",result);

    P1 = (P1 | 0x2A);
    temp8U = P1;
    Y1 = getBit8U(temp8U, 1);
    Y2 = getBit8U(temp8U, 3);
    Y3 = getBit8U(temp8U, 5);
    P2 = (P2 | 0x2A);
    temp8U = P2;
    Y4 = getBit8U(temp8U, 1);
    Y5 = getBit8U(temp8U, 3);
    Y6 = getBit8U(temp8U, 5);

    if (Y1 == A1)
    {
      result = setBit16U(result, 0, 1);
      result = setBit16U(result, 1, 1);
    }
    if (Y2 == A2)
    {
      result = setBit16U(result, 2, 1);
      result = setBit16U(result, 3, 1);
    }
    if (Y3 == A3)
    {
      result = setBit16U(result, 4, 1);
      result = setBit16U(result, 5, 1);
    }
    if (Y4 == A4)
    {
      result = setBit16U(result, 9, 1);
      result = setBit16U(result, 10, 1);
    }
    if (Y5 == A5)
    {
      result = setBit16U(result, 11, 1);
      result = setBit16U(result, 12, 1);
    }
    if (Y6 == A6)
    {
      result = setBit16U(result, 13, 1);
      result = setBit16U(result, 14, 1);
    }
  }
  printf("\n");
  P3 = P3 & 0xC0;
  P1MDOUT = 0;
  P2MDOUT = 0;
  return result;
}

INT16U test00()
{
  INT16U result = 0;
  INT8U A1,B1,Y1,A2,B2,Y2,A3,B3,Y3,A4,B4,Y4;
  INT8U temp8U;
//  INT16U temp16U;
  INT16U looper;
  P1MDOUT = 0x1B;
  P2MDOUT = 0x6C;
  temp8U = P3;
  temp8U = setBit8U(temp8U, 0, 1);
  temp8U = setBit8U(temp8U, 1, 0);
  temp8U = setBit8U(temp8U, 2, 1);
  temp8U = setBit8U(temp8U, 3, 0);
  temp8U = setBit8U(temp8U, 5, 1);
  P3 = temp8U;
  printf("\n");
  for (looper = 0;looper < 0x100;looper++)
  {
    A1 = getBit16U(looper, 0);
    B1 = getBit16U(looper, 1);
    A2 = getBit16U(looper, 2);
    B2 = getBit16U(looper, 3);
    A3 = getBit16U(looper, 4);
    B3 = getBit16U(looper, 5);
    A4 = getBit16U(looper, 6);
    B4 = getBit16U(looper, 7);

    temp8U = P1;
    temp8U = setBit8U(temp8U, 0, A1);
    temp8U = setBit8U(temp8U, 1, B1);
    temp8U = setBit8U(temp8U, 3, A2);
    temp8U = setBit8U(temp8U, 4, B2);
    P1 = temp8U;
    temp8U = P2;
    temp8U = setBit8U(temp8U, 3, A3);
    temp8U = setBit8U(temp8U, 2, B3);
    temp8U = setBit8U(temp8U, 6, A4);
    temp8U = setBit8U(temp8U, 5, B4);
    P2 = temp8U;

    printf("%x\n",result);
    P1 = (P1 | 0x24);
    temp8U = P1;
    Y1 = getBit8U(temp8U, 2);
    Y2 = getBit8U(temp8U, 5);
    P2 = (P2 | 0x12);
    temp8U = P2;
    Y3 = getBit8U(temp8U, 1);
    Y4 = getBit8U(temp8U, 4);

    if (Y1 == (A1 & B1))
    {
      result = setBit16U(result,0,1);
      result = setBit16U(result,1,1);
      result = setBit16U(result,2,1);
    }
    if (Y2 == (A2 & B2))
    {
      result = setBit16U(result,3,1);
      result = setBit16U(result,4,1);
      result = setBit16U(result,5,1);
    }
    if (Y3 == (A3 & B3))
    {
      result = setBit16U(result,9,1);
      result = setBit16U(result,10,1);
      result = setBit16U(result,11,1);
    }
    if (Y4 == (A4 & B4))
    {
      result = setBit16U(result,12,1);
      result = setBit16U(result,13,1);
      result = setBit16U(result,14,1);
    }
  }
  P3 = P3 & 0xC0;
  P1MDOUT = 0;
  P2MDOUT = 0;
  return result;
}

void checkLED()
{
  static INT16U counter = 0;
  static INT16U counter2 = 0;
  INT16U index;
  counter = counter + 1;
  if (counter == 0)
    counter2 = (counter2 + 1) % (0x7);
  if (1)//(counter2 & 0x1 == 0)
  {
    index = counter2;
    switch(index)
    {
    case 0:
      P3 = P3 & 0xE0;
      P4 = (P4 & 0xFC) | 0x2;
      break;
    case 1:
      P3 = P3 & 0xE0;
      P4 = (P4 & 0xFC) | 0x1;
      break;
    case 2:
      P3 = (P3 & 0xE0) | 0x1;
      P4 = (P4 & 0xFC);
      break;
    case 3:
      P3 = (P3 & 0xE0) | 0x2;
      P4 = (P4 & 0xFC);
      break;
    case 4:
      P3 = (P3 & 0xE0) | 0x4;
      P4 = (P4 & 0xFC);
      break;
    case 5:
      P3 = (P3 & 0xE0) | 0x8;
      P4 = (P4 & 0xFC);
      break;
    case 6:
      P3 = (P3 & 0xE0) | 0x10;
      P4 = (P4 & 0xFC);
      break;
    default:
      break;
    }
  }
}