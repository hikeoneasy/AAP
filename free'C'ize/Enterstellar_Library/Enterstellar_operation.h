#include "./Enterstellar_core.h"
/*************************************************************************************************************
*																											 *
*		Enterstellar_operation is used for operations on large integers.									 *
*																											 *
*		Component: Addition, Subtraction, Multiplication, Squaring, Division, Modular Exponentiation.	     *
*																											 *
**************************************************************************************************************/




/*
Bigint Addition : Single-Precision Addition & Multi-Precision Addition.

[Single-Precision Addition]

		ADD_ABC: Addition of Same Signed Word.
			Input: word* x, word* y, int* carry.
			Output: word*C.

[Multi-Precision Addition]

		ADDC: Addition of Same Signed Two Integers.
			Input: bigint* x, bigint* y.
			Output: bigint** z.

		ADD:  Addition of Two Integers.
			Input: bigint* x, bigint* y.
			Output: bigint** z.

*/
// Bigint Addition
void ADD_ABC(word* x, word* y, unsigned int* c, word* C);     // Addition of Same signed Two Word
void ADDC(bigint* x, bigint* y, bigint** z);                 // Addition of Same Signed Two Integers
void ADD(bigint* x, bigint* y, bigint** z);                  // Addition of Two Integers



/*
Bigint Subtraction: Single-Precision Subtraction & Multi-Precision Subtraction.

[Single-Precision Subtraction]

		SUB_AbB: Subtractio of Same signed word.
			Input: word* A, word* B, int* borrow.
			Output: word* C.

[Multi-Precision Subtraction]

		SUBC:  A-B of A >= B > 0.
			Input: bigint* A, bigint* B.
			output: bigint** z.

		SUB: Subtraction of Two Integers.
			Input: bigint* A, bigint* B.
			output: bigint** z.

*/
// Bigint Subtraction
void SUB_AbB(word* A, word* B, unsigned int* b, word* C);			 // Subtraction of Same Signed Word.
void SUBC(bigint* A, bigint* B, bigint** z);				 // Subtraction of Same Signed Two Integers
void SUB(bigint* x, bigint* y, bigint** z);					 // Subtraction of Two Integers.



/*

Bigint Multiplication: Single-Precision Multiplication & Multi-Precision Multiplication.
[Single-Precision Multiplication]
		MUL_AB: Multiplication of word.
			Input: word* x, word* y.
			Output: bigint** z.

[Multi-Precision Multiplication]
	We have two version, MULC_Naive and MULC_Karatsuba. We choose between MULC_Naive and MULC_Karatsuba via flag.
	So, The mul function works using two functions.
	mul -> Karatsuba ( If the flag is not satisfied, you go to Naive version)


		MULC_Naive: Multiplication Naive Version
			Input: bigint* x, bigint* y
			Output: bigint** z


		MULC_Karatsuba: Fast Multiplication Algorithm
			Input: bigint* x, bigint* y
			Output: bigint** z

*/
// Bigint Multiplication 
void MUL_AB(word* x, word* y, bigint** z);                  // Single word Multiplication
void MULC_Naive(bigint* x, bigint* y, bigint** z);          // Multiplication Naive Version 
void MULC_Karatsuba(bigint* x, bigint* y, bigint** z);      // Fast Multiplication Algorithm
void MUL(bigint* x, bigint* y, bigint** z);



/*
Bigint Division: Multiprecision Long Division & 2-Word Long Division

[2-Word Long Divisoin]
	  Long_Division() : Long Division 2-Word Version
		 Input : bigint* A, bigint* B
		 Output : bigint** Q

[Multiprecision Long division]
   We find Q and R from approximate quotient.

	  DIVCC() : Find Q and R from approximate quotient
			( A = n wordlen, B = m wordlen, 0<B<=A<BW, B_m-1 >= 2^(w-1) )
			Input: bigint* A, bigint* B
			Output: bigint** Q, bigint** R

	  DIVC() : Division of Two NON_NEGATIVE integer A, B
			 (A = m+1 wordlen, B = m wordlen, 0 <= A < BW)
			 Input : bigint* A, bigint* B
			 Output : bigint** Q, bigint** R


	  DIV() : Division of Two NON_NEGATIVE integer A, B >0
			Input : bigint* A, bigint* B
			Output : bigint** Q, bigint** R


*/
// Bigint Division
void Long_Division(bigint* A, bigint* B, bigint** Q);
void DIVCC(bigint* A, bigint* B, bigint** Q, bigint** R);
void DIVC(bigint* A, bigint* B, bigint** Q, bigint** R);
void DIV(bigint* A, bigint* B, bigint** Q, bigint** R);



/*

Bigint Squaring : Single-Precision Squaring & Multi-Precision Squaring.

[Single-Precision Squaring]
	Single_percision_sqr
		Input: word A.
		Output: bigint** C

[Multi-Precision Squaring]
	We have three versions. [ SQU, textbook, Karatsuba]
	When the flag is not satisfied, it provides fast squaring processing for inputs 0, 1, -1 and processing for the remaining values.
	If the flag is satisfied, high-speed operation is performed using Karatsuba Squaring.

	SQU: Processing for 0,1,-1
		Input: bigint* x
		Output: bigint** y


	Sqr_Textbook: Squring Naive Version
		Input: bigint* x
		Output: bigint** y

	Sqr_karatsuba: Fast Squring  Version
		Input: bigint* x
		Output: bigint** y

*/
// Bigint Squaring 
void Single_percision_sqr(word A, bigint** C);          //Word Squaring
void Sqr_Textbook(bigint* x, bigint** y);				//Squring Naive Version
void Sqr_karatsuba(bigint* x, bigint** y);				//Fast Squring  Version
void SQU(bigint* x, bigint** y);						//Processing for 0,1,-1




/*

Bigint Modulo Exponentitation : Multiply-and Squaring Method & DIVSION 

Exponentitation: z:= x^n mod M  
Exponentitation2: z:= nx mod M

*-* If you don't know Modulo Multiply-and Squaring Method, Look at this:
Input: x,n,M
output: x^n mod M
	1. t0,t1 <- 1,x
	2. for i <- l-1 down to 0 do
	3.		t(1-ni) <- t0t1 mod M
	4.		t(ni) <- {t(ni)}^2
	5.end for
	6. return t0
*/
//bigint modulo exponential
void Exponentiation(bigint* x, bigint* N, bigint** z, bigint* M);
void Exponentiation2(bigint* x, bigint* N, bigint** z, bigint* M);


