#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <Windows.h>
#include "Enterstellar_operation.h"
#include <time.h>

int main()
{
	srand((unsigned int)time(NULL));
	int t = 0;
	printf("============================This is a calculator using Enterstellar.============================\n");
	Sleep(700);
	printf("\n\n\t\t\tI will show you operations on Big Integers.\n\n");
	Sleep(700);
	printf("\t\t\tWe hope it will help you in your Cryptography study.\n\n");
	Sleep(700);
	printf("\t\t\tLet's randomly generate two large integers.\n\n\n\n");
	bigint* A = NULL;
	bigint_gen_rand(&A, NON_NEGATIVE, 3);
	bigint* B = NULL;
	bigint_gen_rand(&B, NON_NEGATIVE, 2);
	printf("\t\t\tA: "); show_bigint_hex(A); printf("\t\t\tB: ");  show_bigint_hex(B);
	printf("\n\n\t\t\tThen, perform arithmetic operations on A and B.\n\n");
	//t = getch();
	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");

	printf("\t\t\tA: "); show_bigint_hex(A); printf("\t\t\tB: "); show_bigint_hex(B);
	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
	//t = getch();
	bigint* Z = NULL;
	printf("\nThis is Addition A + B\n");
	printf("\n\t\t\tADD(A,B): "); ADD(A, B, &Z); show_bigint_hex(Z);
	
	Sleep(1000);
	printf("\nThis is Subtraction A - B\n");
	printf("\n\t\t\tSUB(A,B): "); SUB(A, B, &Z); show_bigint_hex(Z);

	Sleep(1000);
	printf("\nThis is Multiplication A * B\n");
	printf("\n\t\t\tMUL(A,B): "); MUL(A, B, &Z); show_bigint_hex(Z);

	Sleep(1000);
	printf("\nThis is Division A / B\n");
	bigint* Q = NULL;
	bigint* R = NULL;
	printf("\n\t\t\tDIV(A,B): "); DIV(A, B, &Q, &R); printf(" Q:"); show_bigint_hex(Q); printf("\t\t\tR: "); show_bigint_hex(R);

	Sleep(1000);
	bigint* M = NULL;
	bigint* X = NULL;
	bigint* N = NULL;

	bigint_gen_rand(&M, NON_NEGATIVE, 2);
	
	bigint_gen_rand(&X, NON_NEGATIVE, 2);
	bigint_gen_rand(&N, NON_NEGATIVE, 2);
	printf("\nThis is Modulo Exponentiation\n");
	printf("\t\t\tX: "); show_bigint_hex(X); printf("\t\t\tN: "); show_bigint_hex(N); printf("\t\t\tM: "); show_bigint_hex(M);
	printf("\t\t\tX^N mod M: "); Exponentiation(X, N, &Z,M); show_bigint_hex(Z);
	printf("\t\t\tN*X mod M: "); Exponentiation2(X, N, &Z,M); show_bigint_hex(Z);

	//t = getch();
	return 0;
}