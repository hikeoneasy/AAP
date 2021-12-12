#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Enterstellar_operation.h"
#include <crtdbg.h>
#include <windows.h>
/*
**********************************************************************************************************************************************************************
*                                                                                                                                                                    *
*                       The Enterstellar library was created for arithmetic on large integers.                                                                       *
*                                                                                                                                                                    *
*                       It was produced by Professor Dongchan Kim's class at Kookmin University.                                                                     *
*                                                                                                                                                                    *
*                       creator: Keon-hee Choi , Yong-jin Lee , Hyeon-ho Lee,  Yoon-seo Ki, Dong-chan Kim(not profeesor).                                             *
*                                                                                                                                                                    *
*                       production date:  21. 09. 01. - 21. 12. 08.                                                                                                  *
*                                                                                                                                                                    *
*                                                                                                                                                                    *
*                       Ownership of this code belongs to its creator(Keon-hee Choi , Yong-jin Lee , Hyun-ho Lee,  Yoon-seo Ki, Dong-chan Kim).                     *
*                                                                                                                                                                    *
*                       It is possible to reuse the code for free, but it is prohibited to make it a paid creation.                                                  *
*                                                                                                                                                                    *
*                       It can only be used through a contract with the creator.                                                                                     *
*                                                                                                                                                                    *
*                                                                                                                                                                    *
**********************************************************************************************************************************************************************
*/
void AdditionSpeedTest()
{   
    bigint* A = NULL;
    bigint* B = NULL;

    bigint_gen_rand(&A, NON_NEGATIVE, 64);
    bigint_gen_rand(&B, NON_NEGATIVE, 64);
    bigint* sum = NULL;
    
    LARGE_INTEGER BeginTime, EndTime, Frequency;
    double Time;
    QueryPerformanceFrequency(&Frequency);
    QueryPerformanceCounter(&BeginTime);    
    for (int i = 0; i < 100000; i++){
        ADD(A, B, &sum);    
    }
    QueryPerformanceCounter(&EndTime);
    Time = (double)( EndTime.QuadPart - BeginTime.QuadPart) / Frequency.QuadPart;
    Time /= 100000;
    
    printf("ADD() : %f/msec\n", Time*1000);
    bigint_delete(&sum);
    bigint_delete(&B);
    bigint_delete(&A);
}

void SubtractionSpeedTest()
{   
    bigint* A = NULL;
    bigint* B = NULL;

    bigint_gen_rand(&A, NON_NEGATIVE, 64);
    bigint_gen_rand(&B, NON_NEGATIVE, 64);
    bigint* sub = NULL;
    
    LARGE_INTEGER BeginTime, EndTime, Frequency;
    double Time;
    QueryPerformanceFrequency(&Frequency);
    QueryPerformanceCounter(&BeginTime);    
    for (int i = 0; i < 100000; i++){
        SUB(A, B, &sub);    
    }
    QueryPerformanceCounter(&EndTime);
    Time = (double)( EndTime.QuadPart - BeginTime.QuadPart) / Frequency.QuadPart;
    Time /= 100000;
    
    printf("SUB() : %f/msec\n", Time*1000);
    
    bigint_delete(&sub);
    bigint_delete(&B);
    bigint_delete(&A);
}

void MulNaiveSpeedTest()
{   
    bigint* A = NULL;
    bigint* B = NULL;

    bigint_gen_rand(&A, NON_NEGATIVE, 64);
    bigint_gen_rand(&B, NON_NEGATIVE, 64);
    bigint* mul = NULL;
    
    LARGE_INTEGER BeginTime, EndTime, Frequency;
    double Time;
    QueryPerformanceFrequency(&Frequency);
    QueryPerformanceCounter(&BeginTime);    
    for (int i = 0; i < 1000; i++){
        MULC_Naive(A, B, &mul);    
    }
    QueryPerformanceCounter(&EndTime);
    Time = (double)( EndTime.QuadPart - BeginTime.QuadPart) / Frequency.QuadPart;
    Time /= 1000;
    
    printf("MUL_Naive() : %f/msec\n", Time*1000);
    
    bigint_delete(&mul);
    bigint_delete(&B);
    bigint_delete(&A);
}

void MulKaratsubaSpeedTest()
{   
    bigint* A = NULL;
    bigint* B = NULL;

    bigint_gen_rand(&A, NON_NEGATIVE, 64);
    bigint_gen_rand(&B, NON_NEGATIVE, 64);
    bigint* mul = NULL;
    
    LARGE_INTEGER BeginTime, EndTime, Frequency;
    double Time;
    QueryPerformanceFrequency(&Frequency);
    QueryPerformanceCounter(&BeginTime);    
    for (int i = 0; i < 1000; i++){
        MULC_Karatsuba(A, B, &mul);    
    }
    QueryPerformanceCounter(&EndTime);
    Time = (double)( EndTime.QuadPart - BeginTime.QuadPart) / Frequency.QuadPart;
    Time /= 1000;
    
    printf("MUL_Karatsuba() : %f/msec\n", Time*1000);
    
    bigint_delete(&mul);
    bigint_delete(&B);
    bigint_delete(&A);
}

void SquaringNaiveSpeedTest()
{   
    bigint* A = NULL;
    bigint_gen_rand(&A, NON_NEGATIVE, 64);
    
    bigint* squ = NULL;
    
    LARGE_INTEGER BeginTime, EndTime, Frequency;
    double Time;
    QueryPerformanceFrequency(&Frequency);
    QueryPerformanceCounter(&BeginTime);    
    for (int i = 0; i < 1000; i++){
        Sqr_Textbook(A, &squ);    
    }
    QueryPerformanceCounter(&EndTime);
    Time = (double)( EndTime.QuadPart - BeginTime.QuadPart) / Frequency.QuadPart;
    Time /= 1000;
    
    printf("Sqr_Naive() : %f/msec\n", Time*1000);
    
    bigint_delete(&squ);
    bigint_delete(&A);
}

void SquaringKaratsubaSpeedTest()
{   
    bigint* A = NULL;
    bigint_gen_rand(&A, NON_NEGATIVE, 64);
    
    bigint* squ = NULL;
    
    LARGE_INTEGER BeginTime, EndTime, Frequency;
    double Time;
    QueryPerformanceFrequency(&Frequency);
    QueryPerformanceCounter(&BeginTime);    
    for (int i = 0; i < 1000; i++){
        Sqr_karatsuba(A, &squ);    
    }
    QueryPerformanceCounter(&EndTime);
    Time = (double)( EndTime.QuadPart - BeginTime.QuadPart) / Frequency.QuadPart;
    Time /= 1000;
    
    printf("Sqr_Karatsuba() : %f/msec\n", Time*1000);
    
    bigint_delete(&squ);
    bigint_delete(&A);
}

void DivisionSpeedTest()
{   
    bigint* A = NULL;
    bigint* B = NULL;

    bigint_gen_rand(&A, NON_NEGATIVE, 64);
    bigint_gen_rand(&B, NON_NEGATIVE, 32);
    bigint* Q = NULL;
    bigint* R = NULL; 

    LARGE_INTEGER BeginTime, EndTime, Frequency;
    double Time;
    QueryPerformanceFrequency(&Frequency);
    QueryPerformanceCounter(&BeginTime);    
    for (int i = 0; i < 1000; i++){
        DIV(A, B, &Q, &R);    
    }
    QueryPerformanceCounter(&EndTime);
    Time = (double)( EndTime.QuadPart - BeginTime.QuadPart) / Frequency.QuadPart;
    Time /= 1000;
    
    printf("DIV() : %f/msec\n", Time*1000);
    
    bigint_delete(&R);
    bigint_delete(&Q);
    bigint_delete(&B);
    bigint_delete(&A);
}

void ModularExponentiationSpeedTest()
{   
    bigint* A = NULL;
    bigint_gen_rand(&A, NON_NEGATIVE, 64);

    bigint* N = NULL;
    bigint* M = NULL; 
    bigint_gen_rand(&N, NON_NEGATIVE, 64);
    bigint_gen_rand(&M, NON_NEGATIVE, 64);
    bigint* z = NULL;

    LARGE_INTEGER BeginTime, EndTime, Frequency;
    double Time;
    QueryPerformanceFrequency(&Frequency);
    QueryPerformanceCounter(&BeginTime);    
    for (int i = 0; i < 1; i++){
        Exponentiation(A, N, &z, M);    
    }
    QueryPerformanceCounter(&EndTime);
    Time = (double)( EndTime.QuadPart - BeginTime.QuadPart) / Frequency.QuadPart;
    // Time /= 100000;
    
    printf("Exponentiation1() : %f/sec\n", Time);
    
    bigint_delete(&z);
    bigint_delete(&M);
    bigint_delete(&N);
    bigint_delete(&A);
}

void ModularExponentiation2SpeedTest()
{   
    bigint* A = NULL;
    bigint_gen_rand(&A, NON_NEGATIVE, 64);

    bigint* N = NULL;
    bigint* M = NULL; 
    bigint_gen_rand(&N, NON_NEGATIVE, 64);
    bigint_gen_rand(&M, NON_NEGATIVE, 64);
    bigint* z = NULL;

    LARGE_INTEGER BeginTime, EndTime, Frequency;
    double Time;
    QueryPerformanceFrequency(&Frequency);
    QueryPerformanceCounter(&BeginTime);    
    for (int i = 0; i < 1; i++){
        Exponentiation2(A, N, &z, M);    
    }
    QueryPerformanceCounter(&EndTime);
    Time = (double)( EndTime.QuadPart - BeginTime.QuadPart) / Frequency.QuadPart;
    // Time /= 100000;
    
    printf("Exponentiation2() : %f/sec\n", Time);
    
    bigint_delete(&z);
    bigint_delete(&M);
    bigint_delete(&N);
    bigint_delete(&A);
}

int main()
{


    srand((unsigned int)time(NULL));
    // AdditionSpeedTest();
    // SubtractionSpeedTest();
    // MulNaiveSpeedTest();
    // MulKaratsubaSpeedTest();
    // SquaringNaiveSpeedTest();
    // SquaringKaratsubaSpeedTest();
    // DivisionSpeedTest();
    // ModularExponentiationSpeedTest();
    ModularExponentiation2SpeedTest();
    return 0;
}
