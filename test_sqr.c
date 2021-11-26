#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "./freeCize.h"
#include "./includePython.h"

unsigned char* array2string(bigint* x)
{
    unsigned char* s1 = (unsigned char*)malloc(sizeof(word) * 2 + 1);
    unsigned char* s2 = (unsigned char*)malloc(sizeof(word) * 2 * (x->wordlen) + 1);
    if (s1 != NULL) memset(s1, 0, sizeof(word) * 2 + 1);
    if (s2 != NULL) memset(s2, 0, sizeof(word) * 2 * (x->wordlen) + 1);

    for (int i = (x->wordlen) - 1; i >= 0; i--) {
        if (s1 != 0) sprintf(s1, "%08x", x->a[i]);
        if (s2 != 0 && s1 != 0) strcat(s2, s1);
        if (s1 != NULL) memset(s1, 0, sizeof(word) * 2 + 1);
    }
    free(s1);
    return s2;
}

int main()
{
    /*===========================================================================================================================================*/
    // Setting Up Python for testing only

    PyObject* pName, * pModule, * pFunc, * pValue, * pA, * pB;
    Py_Initialize();                                                        // Python ���������� �ʱ�ȭ 
    if (Py_IsInitialized()) {                                               // Python ���������� �ʱ�ȭ Ȯ�� 
        pName = PyUnicode_FromString("test1");                              // Python File name -> test1.py, �ʱ�ȭ �ʿ�! 
        pModule = PyImport_Import(pName);                                   // Import Python File            �ʱ�ȭ �ʿ�!
        pFunc = PyObject_GetAttrString(pModule, "sqr");                     // test1.py�� �ִ� add �Լ� (= �Լ� �̸�)  �ʱ�ȭ �ʿ�!
        if (PyCallable_Check(pFunc)) {                                      // �Լ��� �� �ҷ��� �Դ��� Ȯ��
            int count = 0;
            printf("Testing : ");
            for (int i = 1; i < 100001; i++) {                              // 100000�� �׽�Ʈ
                bigint* A = NULL;
                bigint_gen_rand(&A, NON_NEGATVE, 1);
                bigint* B = NULL;
                Sqr_karatsuba(A, &B);

                unsigned char* string_A = array2string(A);
                pA = PyUnicode_FromString(string_A);                     // �Ű� ���� ����, c��� ���ڿ��� ���̽� ���ڿ��� ��ȯ 

                unsigned char* string_B = array2string(B);
                pB = PyUnicode_FromString(string_B);

                pValue = PyObject_CallFunctionObjArgs(pFunc, pA, pB, NULL);     
                if (!PyLong_AsLong(pValue)) {
                    printf("\nError");
                    printf("\n");
                    printf("A : ");  show_bigint_hex(A);
                    printf("B : 0x");  show_bigint_hex(B);

                    Py_DECREF(string_A);
                    Py_DECREF(string_B);

                    Py_DECREF(pA);
                    Py_DECREF(pB);
                    Py_DECREF(pValue);

                    bigint_delete(&A);
                    bigint_delete(&B);

                    free(string_A);
                    free(string_B);

                    Py_Finalize();
                    return 0;
                }
                if (i % 1000 == 0) {
                    printf("=");
                }

                Py_DECREF(pA);
                Py_DECREF(pB);
                Py_DECREF(pValue);

                bigint_delete(&A);
                bigint_delete(&B);

                free(string_A);
                free(string_B);

            }
            printf(" : Test Success \n");
        }
        else {
            PyErr_Print();    
        }
        Py_DECREF(pName);
        Py_DECREF(pModule);
        Py_DECREF(pFunc);
    }
    Py_Finalize();         

    return 0;
}

/*
// ����Ʈ�� �����ϴ� ���
pArg1 = PyList_New(A->wordlen);
Py_ssize_t size = PyList_GET_SIZE(pArg1);
for (Py_ssize_t j = 0; j < size; j++) {
    PyList_SetItem(pArg1, j, Py_BuildValue("I", A->a[j]));
}
*/