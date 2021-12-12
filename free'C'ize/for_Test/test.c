#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <Windows.h>
#include "./Enterstellar_operation.h"
#include "./includePython.h"
#include <crtdbg.h>

unsigned char* array2string(bigint* x)
{
    unsigned char* s1 = (unsigned char*)malloc(sizeof(word) * 2 + 1);
    unsigned char* s2 = (unsigned char*)malloc(sizeof(word) * 2 * (x->wordlen) + 1);
    if (s1 != NULL) memset(s1, 0, sizeof(word) * 2 + 1);
    if (s2 != NULL) memset(s2, 0, sizeof(word) * 2 * (x->wordlen) + 1);

#if WordBitLen == 32
    for (int i = (x->wordlen) - 1; i >= 0; i--) {
        if (s1 != 0) sprintf(s1, "%08x", x->a[i]);
        if (s2 != 0 && s1 != 0) strcat(s2, s1);
        if (s1 != NULL) memset(s1, 0, sizeof(word) * 2 + 1);
    }
    free(s1);
    return s2;
#elif WordBitLen == 8
    for (int i = (x->wordlen) - 1; i >= 0; i--) {
        if (s1 != 0) sprintf(s1, "%02x", x->a[i]);
        if (s2 != 0 && s1 != 0) strcat(s2, s1);
        if (s1 != NULL) memset(s1, 0, sizeof(word) * 2 + 1);
}
    free(s1);
    return s2;
#elif WordBitLen == 64
    for (int i = (x->wordlen) - 1; i >= 0; i--) {
        if (s1 != 0) sprintf(s1, "%016llx", x->a[i]);
        if (s2 != 0 && s1 != 0) strcat(s2, s1);
        if (s1 != NULL) memset(s1, 0, sizeof(word) * 2 + 1);
    }
    free(s1);
    return s2;
#endif
    
}

void ADD_test()
{
    PyObject* pName, * pModule, * pFunc, * pValue, * pA, * A_sign, * pB, * B_sign, * pAdd, * Add_sign;
    Py_Initialize();
    if (Py_IsInitialized()) {                                               // Python 인터프리터 초기화 확인 
#if WordBitLen == 8
        pName = PyUnicode_FromString("test_8bit_version");                              // Python File name -> test1.py, 초기화 필요! 
#elif WordBitLen == 32
        pName = PyUnicode_FromString("test_32bit_version");
#elif WordBitLen == 64
        pName = PyUnicode_FromString("test_64bit_version");
#endif
        pModule = PyImport_Import(pName);                                   // Import Python File            초기화 필요!
        pFunc = PyObject_GetAttrString(pModule, "add");                   // test1.py에 있는 add 함수 (= 함수 이름)  초기화 필요!
        if (PyCallable_Check(pFunc)) {                                      // 함수가 잘 불러져 왔는지 확인
            printf("Addition Testing\n");
            printf("Progress : ");
            for (int i = 1; i < 100001; i++) {                              // 100000번 테스트
                bigint* A = NULL;
                bigint* B = NULL;
                bigint_gen_rand(&A, rand() & 0x01, 40);
                bigint_gen_rand(&B, rand() & 0x01, 40);

                bigint* Add = NULL;
                ADD(A, B, &Add);

                unsigned char* string_A = array2string(A);
                pA = PyUnicode_FromString(string_A);                     // 매개 변수 생성, c언어 문자열을 파이썬 문자열로 변환 
                A_sign = Py_BuildValue("i", A->sign);

                unsigned char* string_B = array2string(B);
                pB = PyUnicode_FromString(string_B);
                B_sign = Py_BuildValue("i", B->sign);

                unsigned char* string_Add = array2string(Add);
                pAdd = PyUnicode_FromString(string_Add);
                Add_sign = Py_BuildValue("i", Add->sign);

                // A, A_sign, B, B_sign, sum, sum_sign
                pValue = PyObject_CallFunctionObjArgs(pFunc, pA, A_sign, pB, B_sign, pAdd, Add_sign, NULL);     // 파이썬 mul_k 함수 실행 ( 매개 변수 여러 개 전달 ) 초기화 필요!
                if (!PyLong_AsLong(pValue) || PyLong_AsLong(pValue) == -1) {
                    printf("\nError");
                    printf("\n");

                    if (PyLong_AsLong(pValue) == -1) printf("\nPython Return Error");

                    Py_DECREF(Add_sign);
                    Py_DECREF(pAdd);
                    free(string_Add);

                    Py_DECREF(B_sign);
                    Py_DECREF(pB);
                    free(string_B);

                    Py_DECREF(A_sign);
                    Py_DECREF(pA);
                    free(string_A);

                    bigint_delete(&Add);
                    bigint_delete(&B);
                    bigint_delete(&A);

                    Py_DECREF(pFunc);
                    Py_DECREF(pModule);
                    Py_DECREF(pName);

                    Py_Finalize();
                    return;
                }

                if (i % 1000 == 0) {
                    printf("=");
                }

                Py_DECREF(pValue);

                Py_DECREF(Add_sign);
                Py_DECREF(pAdd);
                free(string_Add);

                Py_DECREF(B_sign);
                Py_DECREF(pB);
                free(string_B);

                Py_DECREF(A_sign);
                Py_DECREF(pA);
                free(string_A);

                bigint_delete(&Add);
                bigint_delete(&B);
                bigint_delete(&A);

            }
            printf("\nAddition Test Success\n\n");
        }           
        else {
            PyErr_Print();  // Python 함수 호출 실패시, 에러 출력    
        }
        Py_DECREF(pFunc);
        Py_DECREF(pModule);
        Py_DECREF(pName);
    }
    Py_Finalize();          // Python 인터프리터 초기화 실패시, 종료 

    return;
}

void SUB_test()
{
    PyObject* pName, * pModule, * pFunc, * pValue, * pA, * A_sign, * pB, * B_sign, * pSub, * Sub_sign;
    Py_Initialize();
    if (Py_IsInitialized()) {    
#if WordBitLen == 8
        pName = PyUnicode_FromString("test_8bit_version");                             
#elif WordBitLen == 32
        pName = PyUnicode_FromString("test_32bit_version");
#elif WordBitLen == 64
        pName = PyUnicode_FromString("test_64bit_version");
#endif
        pModule = PyImport_Import(pName);                                   // Import Python File            초기화 필요! 
        pFunc = PyObject_GetAttrString(pModule, "sub");                   // test1.py에 있는 add 함수 (= 함수 이름)  초기화 필요!
        if (PyCallable_Check(pFunc)) {                                      // 함수가 잘 불러져 왔는지 확인
            printf("Subtraction Testing\n");
            printf("Progress : ");

            for (int i = 1; i < 100001; i++) {                              // 100000번 테스트
                bigint* A = NULL;
                bigint* B = NULL;
                bigint_gen_rand(&A, rand() & 0x01, 40);
                bigint_gen_rand(&B, rand() & 0x01, 40);

                bigint* Sub = NULL;
                SUB(A, B, &Sub);

                unsigned char* string_A = array2string(A);
                pA = PyUnicode_FromString(string_A);                     // 매개 변수 생성, c언어 문자열을 파이썬 문자열로 변환 
                A_sign = Py_BuildValue("i", A->sign);

                unsigned char* string_B = array2string(B);
                pB = PyUnicode_FromString(string_B);
                B_sign = Py_BuildValue("i", B->sign);

                unsigned char* string_Sub = array2string(Sub);
                pSub = PyUnicode_FromString(string_Sub);
                Sub_sign = Py_BuildValue("i", Sub->sign);

                // A, A_sign, B, B_sign, sum, sum_sign
                pValue = PyObject_CallFunctionObjArgs(pFunc, pA, A_sign, pB, B_sign, pSub, Sub_sign, NULL);     // 파이썬 mul_k 함수 실행 ( 매개 변수 여러 개 전달 ) 초기화 필요!
                if (!PyLong_AsLong(pValue) || PyLong_AsLong(pValue) == -1) {
                    printf("\nError");
                    printf("\n");

                    if (PyLong_AsLong(pValue) == -1) printf("\nPython Return Error");

                    Py_DECREF(Sub_sign);
                    Py_DECREF(pSub);
                    free(string_Sub);

                    Py_DECREF(B_sign);
                    Py_DECREF(pB);
                    free(string_B);

                    Py_DECREF(A_sign);
                    Py_DECREF(pA);
                    free(string_A);

                    bigint_delete(&Sub);
                    bigint_delete(&B);
                    bigint_delete(&A);

                    Py_DECREF(pFunc);
                    Py_DECREF(pModule);
                    Py_DECREF(pName);

                    Py_Finalize();
                    return;
                }

                if (i % 1000 == 0) {
                    printf("=");
                }

                Py_DECREF(pValue);

                Py_DECREF(Sub_sign);
                Py_DECREF(pSub);
                free(string_Sub);

                Py_DECREF(B_sign);
                Py_DECREF(pB);
                free(string_B);

                Py_DECREF(A_sign);
                Py_DECREF(pA);
                free(string_A);

                bigint_delete(&Sub);
                bigint_delete(&B);
                bigint_delete(&A);

            }
            printf("\nSubtraction Test Success\n\n");
        }
        else {
            PyErr_Print();  // Python 함수 호출 실패시, 에러 출력    
        }
        Py_DECREF(pFunc);
        Py_DECREF(pModule);
        Py_DECREF(pName);
    }
    Py_Finalize();          // Python 인터프리터 초기화 실패시, 종료 

    return;
}

void MUL_test()
{
    PyObject* pName, * pModule, * pFunc, * pValue, * pA, * A_sign, * pB, * B_sign, * pMul, * Mul_sign;
    Py_Initialize();
    if (Py_IsInitialized()) {                                               // Python 인터프리터 초기화 확인 
#if WordBitLen == 8
        pName = PyUnicode_FromString("test_8bit_version");                              // Python File name -> test1.py, 초기화 필요! 
#elif WordBitLen == 32
        pName = PyUnicode_FromString("test_32bit_version");
#elif WordBitLen == 64
        pName = PyUnicode_FromString("test_64bit_version");
#endif
        pModule = PyImport_Import(pName);                                   // Import Python File            초기화 필요!
        pFunc = PyObject_GetAttrString(pModule, "mul_k");                   // test1.py에 있는 add 함수 (= 함수 이름)  초기화 필요!
        if (PyCallable_Check(pFunc)) {                                      // 함수가 잘 불러져 왔는지 확인
            printf("Multiplication Testing\n");
            printf("Progress : ");
            for (int i = 1; i < 100001; i++) {                              // 100000번 테스트
                bigint* A = NULL;
                bigint* B = NULL;
                bigint_gen_rand(&A, rand() & 0x01, 40);
                bigint_gen_rand(&B, rand() & 0x01, 40);

                bigint* mul = NULL;
                MUL(A, B, &mul);

                unsigned char* string_A = array2string(A);
                pA = PyUnicode_FromString(string_A);                     // 매개 변수 생성, c언어 문자열을 파이썬 문자열로 변환 
                A_sign = Py_BuildValue("i", A->sign);

                unsigned char* string_B = array2string(B);
                pB = PyUnicode_FromString(string_B);
                B_sign = Py_BuildValue("i", B->sign);

                unsigned char* string_mul = array2string(mul);
                pMul = PyUnicode_FromString(string_mul);
                Mul_sign = Py_BuildValue("i", mul->sign);

                // A, A_sign, B, B_sign, sum, sum_sign
                pValue = PyObject_CallFunctionObjArgs(pFunc, pA, A_sign, pB, B_sign, pMul, Mul_sign, NULL);     // 파이썬 mul_k 함수 실행 ( 매개 변수 여러 개 전달 ) 초기화 필요!
                if (!PyLong_AsLong(pValue) || PyLong_AsLong(pValue) == -1) {
                    printf("\nError");
                    printf("\n");
                    
                    if (PyLong_AsLong(pValue) == -1) printf("\nPython Return Error");

                    Py_DECREF(Mul_sign);
                    Py_DECREF(pMul);
                    free(string_mul);

                    Py_DECREF(B_sign);
                    Py_DECREF(pB);
                    free(string_B);
                    
                    Py_DECREF(A_sign);
                    Py_DECREF(pA);
                    free(string_A);

                    bigint_delete(&mul);
                    bigint_delete(&B);
                    bigint_delete(&A);

                    Py_DECREF(pFunc);
                    Py_DECREF(pModule);
                    Py_DECREF(pName);

                    Py_Finalize();
                    return;
                }

                if (i % 1000 == 0) {
                    printf("=");
                }

                Py_DECREF(pValue);

                Py_DECREF(Mul_sign);
                Py_DECREF(pMul);
                free(string_mul);

                Py_DECREF(B_sign);
                Py_DECREF(pB);
                free(string_B);

                Py_DECREF(A_sign);
                Py_DECREF(pA);
                free(string_A);

                bigint_delete(&mul);
                bigint_delete(&B);
                bigint_delete(&A);

            }
            printf("\nMultiplication Test Success\n\n");
        }
        else {
            PyErr_Print();  // Python 함수 호출 실패시, 에러 출력    
        }
        Py_DECREF(pFunc);
        Py_DECREF(pModule);
        Py_DECREF(pName);
    }
    Py_Finalize();          // Python 인터프리터 초기화 실패시, 종료 

    return;

}

void SQU_test()
{
    PyObject* pName, * pModule, * pFunc, * pValue, * pA, * A_sign, * pSqu, * Squ_sign;
    Py_Initialize();
    if (Py_IsInitialized()) {                                               // Python 인터프리터 초기화 확인 
        pName = PyUnicode_FromString("test1");                              // Python File name -> test1.py, 초기화 필요! 
#if WordBitLen == 8
        pName = PyUnicode_FromString("test_8bit_version");                              // Python File name -> test1.py, 초기화 필요! 
#elif WordBitLen == 32
        pName = PyUnicode_FromString("test_32bit_version");
#elif WordBitLen == 64
        pName = PyUnicode_FromString("test_64bit_version");
#endif
        pModule = PyImport_Import(pName);                                   // Import Python File            초기화 필요!
        pFunc = PyObject_GetAttrString(pModule, "sqr");                   // test1.py에 있는 add 함수 (= 함수 이름)  초기화 필요!
        if (PyCallable_Check(pFunc)) {                                      // 함수가 잘 불러져 왔는지 확인
            printf("Squaring Testing\n");
            printf("Progress : ");
            for (int i = 1; i < 100001; i++) {                              // 100000번 테스트
                bigint* A = NULL;
                bigint_gen_rand(&A, rand() & 0x01, 40);

                bigint* Squ = NULL;
                SQU(A, &Squ);

                unsigned char* string_A = array2string(A);
                pA = PyUnicode_FromString(string_A);                     // 매개 변수 생성, c언어 문자열을 파이썬 문자열로 변환 
                A_sign = Py_BuildValue("i", A->sign);

                unsigned char* string_Squ = array2string(Squ);
                pSqu = PyUnicode_FromString(string_Squ);
                Squ_sign = Py_BuildValue("i", Squ->sign);

                // A, A_sign, B, B_sign, sum, sum_sign
                pValue = PyObject_CallFunctionObjArgs(pFunc, pA, A_sign, pSqu, Squ_sign, NULL);     // 파이썬 mul_k 함수 실행 ( 매개 변수 여러 개 전달 ) 초기화 필요!
                if (!PyLong_AsLong(pValue) || PyLong_AsLong(pValue) == -1) {
                    printf("\nError");
                    printf("\n");

                    if (PyLong_AsLong(pValue) == -1) printf("\nPython Return Error");

                    Py_DECREF(Squ_sign);
                    Py_DECREF(pSqu);
                    free(string_Squ);

                    Py_DECREF(A_sign);
                    Py_DECREF(pA);
                    free(string_A);

                    bigint_delete(&Squ);
                    bigint_delete(&A);

                    Py_DECREF(pFunc);
                    Py_DECREF(pModule);
                    Py_DECREF(pName);

                    Py_Finalize();
                    return;
                }

                if (i % 1000 == 0) {
                    printf("=");
                }

                Py_DECREF(pValue);

                Py_DECREF(Squ_sign);
                Py_DECREF(pSqu);
                free(string_Squ);

                Py_DECREF(A_sign);
                Py_DECREF(pA);
                free(string_A);

                bigint_delete(&Squ);
                bigint_delete(&A);

            }
            printf("\nSquaring Test Success\n\n");
        }
        else {
            PyErr_Print();  // Python 함수 호출 실패시, 에러 출력    
        }
        Py_DECREF(pFunc);
        Py_DECREF(pModule);
        Py_DECREF(pName);
    }
    Py_Finalize();          // Python 인터프리터 초기화 실패시, 종료 

    return;

}

void DIV_test()
{
    PyObject* pName, * pModule, * pFunc, * pValue, * pA, * A_sign, * pB, * B_sign, * pQ, * Q_sign, * pR, * R_sign;
    Py_Initialize();
    if (Py_IsInitialized()) {                                               // Python 인터프리터 초기화 확인 
#if WordBitLen == 8
        pName = PyUnicode_FromString("test_8bit_version");                              // Python File name -> test1.py, 초기화 필요! 
#elif WordBitLen == 32
        pName = PyUnicode_FromString("test_32bit_version");
#elif WordBitLen == 64
        pName = PyUnicode_FromString("test_64bit_version");
#endif
        pModule = PyImport_Import(pName);                                   // Import Python File            초기화 필요!
        pFunc = PyObject_GetAttrString(pModule, "div");                   // test1.py에 있는 add 함수 (= 함수 이름)  초기화 필요!
        if (PyCallable_Check(pFunc)) {                                      // 함수가 잘 불러져 왔는지 확인
            printf("Division Testing\n");
            printf("Progress : ");
            for (int i = 1; i < 100001; i++) {                              // 100000번 테스트
                bigint* A = NULL;
                bigint* B = NULL;
                bigint_gen_rand(&A, NON_NEGATIVE, 40);
                bigint_gen_rand(&B, NON_NEGATIVE, 20);
                while (IsZero(B)) {
                    bigint* B_ = NULL;
                    bigint_gen_rand(&B_, NON_NEGATIVE, 1);
                    bigint_assign(&B, B_);
                    bigint_delete(&B_);
                }
                bigint* Q = NULL;
                bigint* R = NULL;
                DIV(A, B, &Q, &R);

                unsigned char* string_A = array2string(A);
                pA = PyUnicode_FromString(string_A);                     // 매개 변수 생성, c언어 문자열을 파이썬 문자열로 변환 
                A_sign = Py_BuildValue("i", A->sign);

                unsigned char* string_B = array2string(B);
                pB = PyUnicode_FromString(string_B);
                B_sign = Py_BuildValue("i", B->sign);

                unsigned char* string_Q = array2string(Q);
                pQ = PyUnicode_FromString(string_Q);
                Q_sign = Py_BuildValue("i", Q->sign);

                unsigned char* string_R = array2string(R);
                pR = PyUnicode_FromString(string_R);
                R_sign = Py_BuildValue("i", R->sign);

                // A, A_sign, B, B_sign, sum, sum_sign
                pValue = PyObject_CallFunctionObjArgs(pFunc, pA, A_sign, pB, B_sign, pQ, Q_sign, pR, R_sign, NULL);     // 파이썬 mul_k 함수 실행 ( 매개 변수 여러 개 전달 ) 초기화 필요!
                if (!PyLong_AsLong(pValue)) {
                    printf("\nError");
                    printf("\n");

                    Py_DECREF(pValue);

                    Py_DECREF(R_sign);
                    Py_DECREF(pR);
                    free(string_R);

                    Py_DECREF(Q_sign);
                    Py_DECREF(pQ);
                    free(string_Q);

                    Py_DECREF(B_sign);
                    Py_DECREF(pB);
                    free(string_B);

                    Py_DECREF(A_sign);
                    Py_DECREF(pA);
                    free(string_A);

                    bigint_delete(&R);
                    bigint_delete(&Q);
                    bigint_delete(&B);
                    bigint_delete(&A);

                    Py_DECREF(pFunc);
                    Py_DECREF(pModule);
                    Py_DECREF(pName);

                    Py_Finalize();
                    return;
                }

                if (i % 1000 == 0) {
                    printf("=");
                }

                Py_DECREF(pValue);

                Py_DECREF(R_sign);
                Py_DECREF(pR);
                free(string_R);

                Py_DECREF(Q_sign);
                Py_DECREF(pQ);
                free(string_Q);

                Py_DECREF(B_sign);
                Py_DECREF(pB);
                free(string_B);

                Py_DECREF(A_sign);
                Py_DECREF(pA);
                free(string_A);

                bigint_delete(&R);
                bigint_delete(&Q);
                bigint_delete(&B);
                bigint_delete(&A);

            }
            printf("\nDivision Test Success\n\n");
        }
        else {
            PyErr_Print();  // Python 함수 호출 실패시, 에러 출력    
        }
        Py_DECREF(pFunc);
        Py_DECREF(pModule);
        Py_DECREF(pName);
    }
    Py_Finalize();          // Python 인터프리터 초기화 실패시, 종료 

    return;
}

void ModularExponentiation1_test()
{
    PyObject* pName, * pModule, * pFunc, * pValue, * pA, * A_sign, * pN, * N_sign, * pB, * B_sign, *pM, *M_sign;
    Py_Initialize();
    if (Py_IsInitialized()) {                                               // Python 인터프리터 초기화 확인 
#if WordBitLen == 8
        pName = PyUnicode_FromString("test_8bit_version");                              // Python File name -> test1.py, 초기화 필요! 
#elif WordBitLen == 32
        pName = PyUnicode_FromString("test_32bit_version");
#elif WordBitLen == 64
        pName = PyUnicode_FromString("test_64bit_version");
#endif
        pModule = PyImport_Import(pName);                                   // Import Python File            초기화 필요!
        pFunc = PyObject_GetAttrString(pModule, "Modular_exponentiation1");                   // test1.py에 있는 add 함수 (= 함수 이름)  초기화 필요!
        if (PyCallable_Check(pFunc)) {                                      // 함수가 잘 불러져 왔는지 확인
            printf("Modular Exponentiation Testing\n");
            printf("Progress : ");
            for (int i = 1; i < 100001; i++) {                              // 100000번 테스트
                bigint* A = NULL;
                bigint* N = NULL;
                bigint* M = NULL;
                bigint_gen_rand(&A, NON_NEGATIVE, 1);
                bigint_gen_rand(&N, NON_NEGATIVE, 1);
                bigint_gen_rand(&M, NON_NEGATIVE, 1);
                while (IsZero(M)) {
                    bigint* M_ = NULL;
                    bigint_gen_rand(&M_, NON_NEGATIVE, 1);
                    bigint_assign(&M, M_);
                    bigint_delete(&M_);
                }
                bigint* B = NULL;
                Exponentiation(A, N, &B, M);

                unsigned char* string_A = array2string(A);
                pA = PyUnicode_FromString(string_A);                     // 매개 변수 생성, c언어 문자열을 파이썬 문자열로 변환 
                A_sign = Py_BuildValue("i", A->sign);

                unsigned char* string_N = array2string(N);
                pN = PyUnicode_FromString(string_N);
                N_sign = Py_BuildValue("i", N->sign);

                unsigned char* string_M = array2string(M);
                pM = PyUnicode_FromString(string_M);
                M_sign = Py_BuildValue("i", M->sign);

                unsigned char* string_B = array2string(B);
                pB = PyUnicode_FromString(string_B);
                B_sign = Py_BuildValue("i", B->sign);

                // A, A_sign, B, B_sign, sum, sum_sign
                pValue = PyObject_CallFunctionObjArgs(pFunc, pA, A_sign, pN, N_sign, pB, B_sign, pM, M_sign, NULL);     // 파이썬 mul_k 함수 실행 ( 매개 변수 여러 개 전달 ) 초기화 필요!
                if (!PyLong_AsLong(pValue) || PyLong_AsLong(pValue) == -1) {
                    printf("\nError");
                    printf("\n");

                    if (PyLong_AsLong(pValue) == -1) printf("\nPython Return Error");

                    Py_DECREF(B_sign);
                    Py_DECREF(pB);
                    free(string_B);

                    Py_DECREF(M_sign);
                    Py_DECREF(pM);
                    free(string_M);

                    Py_DECREF(N_sign);
                    Py_DECREF(pN);
                    free(string_N);

                    Py_DECREF(A_sign);
                    Py_DECREF(pA);
                    free(string_A);

                    bigint_delete(&B);
                    bigint_delete(&M);
                    bigint_delete(&N);
                    bigint_delete(&A);

                    Py_DECREF(pFunc);
                    Py_DECREF(pModule);
                    Py_DECREF(pName);

                    Py_Finalize();
                    return;
                }

                if (i % 1000 == 0) {
                    printf("=");
                }

                Py_DECREF(B_sign);
                Py_DECREF(pB);
                free(string_B);

                Py_DECREF(M_sign);
                Py_DECREF(pM);
                free(string_M);

                Py_DECREF(N_sign);
                Py_DECREF(pN);
                free(string_N);

                Py_DECREF(A_sign);
                Py_DECREF(pA);
                free(string_A);

                bigint_delete(&B);
                bigint_delete(&M);
                bigint_delete(&N);
                bigint_delete(&A);

            }
            printf("\nModular Exponentiation Test Success\n\n");
        }
        else {
            PyErr_Print();  // Python 함수 호출 실패시, 에러 출력    
        }
        Py_DECREF(pFunc);
        Py_DECREF(pModule);
        Py_DECREF(pName);
    }
    Py_Finalize();          // Python 인터프리터 초기화 실패시, 종료 

    return;

}

void ModularExponentiation2_test() {
    PyObject* pName, * pModule, * pFunc, * pValue, * pA, * A_sign, * pN, * N_sign, * pB, * B_sign, * pM, * M_sign;
    Py_Initialize();
    if (Py_IsInitialized()) {                                               // Python 인터프리터 초기화 확인 
#if WordBitLen == 8
        pName = PyUnicode_FromString("test_8bit_version");                              // Python File name -> test1.py, 초기화 필요! 
#elif WordBitLen == 32
        pName = PyUnicode_FromString("test_32bit_version");
#elif WordBitLen == 64
        pName = PyUnicode_FromString("test_64bit_version");
#endif
        pModule = PyImport_Import(pName);                                   // Import Python File            초기화 필요!
        pFunc = PyObject_GetAttrString(pModule, "Modular_exponentiation2");                   // test1.py에 있는 add 함수 (= 함수 이름)  초기화 필요!
        if (PyCallable_Check(pFunc)) {                                      // 함수가 잘 불러져 왔는지 확인
            printf("Modular Exponentiation2 Testing\n");
            printf("Progress : ");
            for (int i = 1; i < 100001; i++) {                              // 100000번 테스트
                bigint* A = NULL;
                bigint* N = NULL;
                bigint* M = NULL;
                bigint_gen_rand(&A, NON_NEGATIVE, 1);
                bigint_gen_rand(&N, NON_NEGATIVE, 1);
                bigint_gen_rand(&M, NON_NEGATIVE, 1);
                while (IsZero(M)) {
                    bigint* M_ = NULL;
                    bigint_gen_rand(&M_, NON_NEGATIVE, 1);
                    bigint_assign(&M, M_);
                    bigint_delete(&M_);
                }
                bigint* B = NULL;
                Exponentiation2(A, N, &B, M);

                unsigned char* string_A = array2string(A);
                pA = PyUnicode_FromString(string_A);                     // 매개 변수 생성, c언어 문자열을 파이썬 문자열로 변환 
                A_sign = Py_BuildValue("i", A->sign);

                unsigned char* string_N = array2string(N);
                pN = PyUnicode_FromString(string_N);
                N_sign = Py_BuildValue("i", N->sign);

                unsigned char* string_M = array2string(M);
                pM = PyUnicode_FromString(string_M);
                M_sign = Py_BuildValue("i", M->sign);

                unsigned char* string_B = array2string(B);
                pB = PyUnicode_FromString(string_B);
                B_sign = Py_BuildValue("i", B->sign);

                // A, A_sign, B, B_sign, sum, sum_sign
                pValue = PyObject_CallFunctionObjArgs(pFunc, pA, A_sign, pN, N_sign, pB, B_sign, pM, M_sign, NULL);     // 파이썬 mul_k 함수 실행 ( 매개 변수 여러 개 전달 ) 초기화 필요!
                if (!PyLong_AsLong(pValue) || PyLong_AsLong(pValue) == -1) {
                    printf("\nError");
                    printf("\n");

                    if (PyLong_AsLong(pValue) == -1) printf("\nPython Return Error");

                    Py_DECREF(B_sign);
                    Py_DECREF(pB);
                    free(string_B);

                    Py_DECREF(M_sign);
                    Py_DECREF(pM);
                    free(string_M);

                    Py_DECREF(N_sign);
                    Py_DECREF(pN);
                    free(string_N);

                    Py_DECREF(A_sign);
                    Py_DECREF(pA);
                    free(string_A);

                    bigint_delete(&B);
                    bigint_delete(&M);
                    bigint_delete(&N);
                    bigint_delete(&A);

                    Py_DECREF(pFunc);
                    Py_DECREF(pModule);
                    Py_DECREF(pName);

                    Py_Finalize();
                    return;
                }

                if (i % 1000 == 0) {
                    printf("=");
                }

                Py_DECREF(B_sign);
                Py_DECREF(pB);
                free(string_B);

                Py_DECREF(M_sign);
                Py_DECREF(pM);
                free(string_M);

                Py_DECREF(N_sign);
                Py_DECREF(pN);
                free(string_N);

                Py_DECREF(A_sign);
                Py_DECREF(pA);
                free(string_A);

                bigint_delete(&B);
                bigint_delete(&M);
                bigint_delete(&N);
                bigint_delete(&A);

            }
            printf("\nModular Exponentiation2 Test Success\n\n");
        }
        else {
            PyErr_Print();  // Python 함수 호출 실패시, 에러 출력    
        }
        Py_DECREF(pFunc);
        Py_DECREF(pModule);
        Py_DECREF(pName);
    }
    Py_Finalize();          // Python 인터프리터 초기화 실패시, 종료 

    return;
}

int main()
{   
    /*=========================================================================================================================================================*/
    // Setting Up Python for testing only
    srand((unsigned int)time(NULL));
    printf("<                                           %d-bit Version Testing                                   >\n\n", WordBitLen);
    ADD_test();
    SUB_test();
    MUL_test();
    SQU_test();
    DIV_test();
    ModularExponentiation1_test();
    ModularExponentiation2_test();
    printf("<                                              Test Complete!!!                                      >\n");
    return 0;
}

                /*
                // 리스트로 전달하는 방법
                pArg1 = PyList_New(A->wordlen);
                Py_ssize_t size = PyList_GET_SIZE(pArg1);

                for (Py_ssize_t j = 0; j < size; j++) {
                    PyList_SetItem(pArg1, j, Py_BuildValue("I", A->a[j]));
                }
                */
