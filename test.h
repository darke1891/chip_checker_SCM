#ifndef __TEST_H__
#define __TEST_H__
#include "myDefine.h"
//Tests
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