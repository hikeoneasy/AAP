#include "freeCize.h"

// ADDITION //

void ADD_ABC(word* x, word* y, int* c, word* C)
{   
    int carry = (*c);
    (*c) = 0;
    (*C) = ((*x) + (*y));               // (A + B) mod 2 ^ WordBitLen
    if ((*C) < (*x)) (*c) = 1;    

    (*C) = ((*C) + carry);
    if ((*C) < carry) (*c) = 1;

    carry = 0;
}


void ADDC(bigint* x, bigint* y, bigint** z)
{   
    y->a = (word*)realloc(y->a, sizeof(word)*(x->wordlen));
    for (int j = y->wordlen; j < x->wordlen; j++){
        y->a[j] = 0;
    }
    word C = 0;                                 // C is (x[j] + y[j]) mod 2 ^ WordBitLen
    bigint* sum = NULL;                         // a + b 저장할 sum 생성 
    bigint_create(&sum, x->wordlen + 1);        // To save A + B, 최대 max(n, m) + 1 wordlen need
    int carry = 0;

    for (int j = 0; j < x->wordlen; j++){                       // Updating carry and C
        ADD_ABC(&(x->a[j]), &(y->a[j]), &carry, &C);
        sum->a[j] = C;
        // show_bigint_hex(sum);
        C = 0;
    }
    
    if (carry == 1){                    // If last carry is 1, sum = [1][word][word]...[word]
        sum->a[sum->wordlen - 1] = 1;
    }

    bigint_refine(sum);                 // reallocation bigint sum
    bigint_refine(y);
    bigint_assign(z, sum);    
    bigint_delete(&sum);
}

void ADD(bigint* x, bigint* y, bigint** z)
{   
    if (IsZero(x)){
        bigint_assign(z, y);
        return;
    }

    if (IsZero(y)){
        bigint_assign(z, x);
        return;
    } 

    if ((!IsZero(x) && x->sign == NON_NEGATVE) && (!IsZero(y) && y->sign == NEGATIVE)){
        bigint* y_ = NULL;
        bigint_assign(&y_, y);
        y_->sign = NON_NEGATVE;
        SUB(x, y_, z);
        bigint_delete(&y_);
        return;
    }
 
    if ((!IsZero(y) && y->sign == NON_NEGATVE) && (!IsZero(x) && x->sign == NEGATIVE)){
        bigint* x_ = NULL;
        bigint_assign(&x_, x);
        x_->sign = NON_NEGATVE;
        SUB(y, x_, z);
        bigint_delete(&x_);
        return;
    }

    if (x->wordlen >= y->wordlen){
        ADDC(x, y, z);
        return;
    }

    if (x->wordlen < y->wordlen){
        ADDC(y, x, z);
        return;
    } 
}

//SUBTRACT//
void SUB_AbB(word* A, word* B, int* b, word* C)
{
    int b_2 = 0;
    (*C) = ((*A) - (*b));              
    if ((*A) < (*b)) 
        b_2 = 1;

    if ((*C) < (*B)) 
        b_2 = (b_2) ^ 0x1;
    (*C) = (*C) - (*B);
    *b = b_2;
}

void SUBC(bigint* A, bigint* B, bigint** z)
{
    B->a = (word*)realloc(B->a, sizeof(word) * (A->wordlen));
    for (int j = B->wordlen; j < A->wordlen; j++) {
        B->a[j] = 0;
    }

    int b = 0;                              
    word C = 0;                              
    bigint* sub = NULL;
    bigint_create(&sub, A->wordlen);       

    for (int j = 0; j < A->wordlen; j++) {                       
        SUB_AbB(&(A->a[j]), &(B->a[j]), &b, &C);
        sub->a[j] = C;
        C = 0;
    }

    bigint_refine(sub);
    bigint_refine(B);
    bigint_assign(z, sub);
    bigint_delete(&sub);
}

void SUB(bigint* x, bigint* y, bigint** z)
{
    if (IsZero(x)){
        bigint_assign(z, y);
        (*z)->sign = NEGATIVE; 
        return;
    }
    if (IsZero(y)){
        bigint_assign(z, x);
        (*z)->sign = NON_NEGATVE; 
        return;
    }
    if(Compare(x,y)==0){
        bigint_set_zero(z);
        return;
    }
    if((y->sign==NON_NEGATVE)&&(x->sign==NON_NEGATVE)&&(Compare(x,y) != -1) )
    {
        SUBC(x,y,z);
        (*z)->sign = NON_NEGATVE;
        return;
    }
    if((x->sign == NON_NEGATVE)&&(y->sign==NON_NEGATVE)&&(Compare(x,y)==-1))
    {
        SUBC(y,x,z);
        (*z)->sign = NEGATIVE;
        return;
    }
    if((x->sign==NEGATIVE)&&(y->sign==NEGATIVE)&&(Compare(x,y)!=-1))
    {
        bigint* x_ = NULL;
        bigint* y_ = NULL;
        bigint_assign(&x_, x);
        bigint_assign(&y_, y);    
        x_->sign = NON_NEGATVE;
        y_->sign = NON_NEGATVE;
        SUBC(y_,x_,z);
        bigint_delete(&x_);
        bigint_delete(&y_);
        return;
    }
    if((y->sign==NEGATIVE)&&(x->sign==NEGATIVE)&&(Compare(x,y)==-1))
    {
        bigint* x_ = NULL;
        bigint* y_ = NULL;
        bigint_assign(&x_, x);
        bigint_assign(&y_, y);    
        x_->sign = NON_NEGATVE;
        y_->sign = NON_NEGATVE;
        SUBC(x,y,z);
        (*z)->sign = NEGATIVE;
        bigint_delete(&x_);
        bigint_delete(&y_);
        return;
    }

    if((x->sign == NON_NEGATVE)&&(y->sign == NEGATIVE))
    {
        bigint* y_ = NULL;
        bigint_assign(&y_, y);
        y_->sign = NON_NEGATVE;
        ADD(x,y,z);
        bigint_delete(&y_);

        return;
    }
    if((x->sign == NEGATIVE)&&(y->sign == NON_NEGATVE))
    {   
        bigint* x_ = NULL;
        bigint_assign(&x_, y);
        x_->sign = NON_NEGATVE;
        ADD(x,y,z);
        (*z)->sign = NEGATIVE;        
        bigint_delete(&x_);
        return;
    }
}

// Multiplication 
void MUL_AB(word* x, word* y, bigint** z)
{   
    bigint* mul = NULL;
    bigint_create(&mul, 2);

    word A1 = (*x) >> (WordBitLen / 2); 
    word A0 = (*x) & (word)((1 << (WordBitLen / 2)) - 1);
    
    word B1 = (*y) >> (WordBitLen / 2); 
    word B0 = (*y) & (word)((1 << (WordBitLen / 2)) - 1);
    
    word T1 = A1 * B0;
    word T0 = A0 * B1;
    word T = 0;

    T0 = T1 + T0;
    T1 = (T0 < T1);

    word C1 = A1 * B1;
    word C0 = A0 * B0;
    T = C0;

    C0 = C0 + (T0 << (WordBitLen / 2));
    C1 = C1 + (T1 << (WordBitLen / 2)) + (T0 >> (WordBitLen / 2)) + (C0 < T);

    mul->a[1] = C1;
    mul->a[0] = C0;

    bigint_assign(z, mul);
    bigint_delete(&mul);
    A1 = 0; A0 = 0; B1 = 0; B1 = 0; 
    T1 = 0; T0 = 0; T = 0;
    C1 = 0; C0 = 0; 
}

void MULC_Naive(bigint* x, bigint* y, bigint** z)
{
    bigint* mul = NULL;
    bigint_create(&mul, (x->wordlen + y->wordlen));
    mul->a[0] = 0;

    for (int i = 0; i < x->wordlen; i++){
        for (int j = 0; j < y->wordlen; j++){
            bigint* T = NULL;
            MUL_AB(&(x->a[i]), &(y->a[j]), &T);       // 여기 수정함 11/15 AM11:50
            LeftShift(T, WordBitLen * (i + j));
            if (T->wordlen <= mul->wordlen) ADDC(mul, T, &mul);
            else ADDC(T, mul, &mul);
            bigint_delete(&T);
        }
    }

    bigint_assign(z, mul);
    bigint_delete(&mul); 
}

void MULC_Karatsuba(bigint* x, bigint* y, bigint** z)
{   
    // wordlen이 충분히 길지 않으면 Naive Version으로 연산하는 것이 더 빠르다
    if (x->wordlen <= 10 | y->wordlen <= 10){
        MULC_Naive(x, y, z);
        return;
    }
    else{
        int l = 0;
        if (x->wordlen >= y->wordlen) l = (x->wordlen + 1) >> 1;
        if (x->wordlen < y->wordlen) l = (y->wordlen + 1) >> 1;
        // printf("l : %d \n", l);
        bigint* A1 = NULL; bigint* A0 = NULL;
        bigint* B1 = NULL; bigint* B0 = NULL;
        bigint* T1 = NULL; bigint* T0 = NULL;
        bigint* S1 = NULL; bigint* S0 = NULL;
        bigint* S = NULL; bigint* R = NULL;

        bigint_assign(&A1, x);
        bigint_assign(&A0, x);
        bigint_assign(&B1, y);
        bigint_assign(&B0, y);
        
        RightShift(A1, l * WordBitLen);
        Reduction(A0, l * WordBitLen);
        RightShift(B1, l * WordBitLen);
        Reduction(B0, l * WordBitLen);
    
        // printf("A1 : (%d)", A1->sign); show_bigint_hex(A1);
        // printf("A0 : (%d)", A0->sign); show_bigint_hex(A0);
        // printf("B1 : (%d)", B1->sign); show_bigint_hex(B1);
        // printf("B0 : (%d)", B0->sign); show_bigint_hex(B0);
        // printf("\n");

        
        MULC_Karatsuba(A1, B1, &T1);
        MULC_Karatsuba(A0, B0, &T0);
        // printf("T1 : (%d)", T1->sign); show_bigint_hex(T1);
        // printf("T0 : (%d)", T0->sign); show_bigint_hex(T0);
        // printf("\n");
        
        LeftShift(T1, 2 * l * WordBitLen);
        ADD(T1, T0, &R);
        RightShift(T1, 2 * l * WordBitLen);
        // printf("T1 + T0 : (%d)", R->sign); show_bigint_hex(R);
        // printf("T1 : (%d)", T1->sign); show_bigint_hex(T1);

        SUB(A0, A1, &S1);
        SUB(B1, B0, &S0);
        // printf("S1 : (%d) ", S1->sign); show_bigint_hex(S1);
        // printf("S0 : (%d) ", S0->sign); show_bigint_hex(S0);

        int S_sign = S1->sign ^ S0->sign;
        S1->sign = NON_NEGATVE; S0->sign = NON_NEGATVE;
        MULC_Karatsuba(S1, S0, &S);
        if (S_sign == 0) S->sign = NON_NEGATVE;
        if (S_sign == 1) S->sign = NEGATIVE;
        // printf("S : (%d) ", S->sign); show_bigint_hex(S);

        ADD(S, T1, &S);  
        // printf("S + T1 : (%d) ", S->sign); show_bigint_hex(S);
        ADD(S, T0, &S);
        // printf("S + T1 + T0: (%d) ", S->sign); show_bigint_hex(S);

        // Error...;; 
        LeftShift(S, l * WordBitLen);
        // printf("S : (%d) ", S->sign); show_bigint_hex(S);

        ADD(R, S, &R);
        // printf("R : (%d) ", R->sign); show_bigint_hex(R);
        bigint_refine(R);
        bigint_assign(z, R);
        
        bigint_delete(&A1); bigint_delete(&A0);
        bigint_delete(&B1); bigint_delete(&B0);
        bigint_delete(&T1); bigint_delete(&T0);
        bigint_delete(&S1); bigint_delete(&S0);
        bigint_delete(&S); bigint_delete(&R);
    }
}
