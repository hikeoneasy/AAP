#include "Enterstellar_operation.h"

// ADDITION //

void ADD_ABC(word* x, word* y, unsigned int* c, word* C)
{   
    unsigned int carry = (*c);
    (*c) = 0;
    (*C) = ((*x) + (*y));               // (A + B) mod 2 ^ WordBitLen
    if ((*C) < (*x)) (*c) = 1;    

    (*C) = ((*C) + carry);
    if ((*C) < carry) (*c) = 1;

    carry = 0;
}

void ADDC(bigint* x, bigint* y, bigint** z)
{   
    word* tmp = NULL;
    tmp = y->a;
    y->a = (word*)realloc(y->a, sizeof(word)*(x->wordlen));
    if (y->a == NULL) {
        printf("Memory Reallocation Fail\n");
        y->a = tmp;
        return;
    }
    for (int j = y->wordlen; j < x->wordlen; j++){
        y->a[j] = 0;
    }
    word C = 0;                                 // C is (x[j] + y[j]) mod 2 ^ WordBitLen
    bigint* sum = NULL;                         // a + b -> sum
    bigint_create(&sum, x->wordlen + 1);        // To save A + B, max(n, m) + 1 wordlen need
    unsigned int carry = 0;

    for (int j = 0; j < x->wordlen; j++){                       // Updating carry and C
        ADD_ABC(&(x->a[j]), &(y->a[j]), &carry, &C);
        sum->a[j] = C;
        C = 0;
    }
    
    if (carry == 1){                    // If last carry is 1, sum = [1][word][word]...[word]
        sum->a[sum->wordlen - 1] = 1;
    }

    bigint_refine(sum);                 // reallocation bigint sum
    bigint_refine(y);
    bigint_assign(z, sum); 
    (*z)->sign = x->sign;   
    bigint_delete(&sum);
}

void ADD(bigint* x, bigint* y, bigint** z)
{   
    bigint* x_ = NULL;
    bigint* y_ = NULL;

    bigint_assign(&x_, x);
    bigint_assign(&y_, y);

    if (IsZero(x_)){
        bigint_assign(z, y_);
        bigint_delete(&y_);
        bigint_delete(&x_);
        return;
    }

    if (IsZero(y_)){
        bigint_assign(z, x_);
        bigint_delete(&y_);
        bigint_delete(&x_);
        return;
    } 

    if ((!IsZero(x_) && x_->sign == NON_NEGATIVE) && (!IsZero(y_) && y_->sign == NEGATIVE)){
        y_->sign = NON_NEGATIVE;
        SUB(x_, y_, z);
        bigint_delete(&y_);
        bigint_delete(&x_);
        return;
    }
 
    if ((!IsZero(y_) && y_->sign == NON_NEGATIVE) && (!IsZero(x_) && x_->sign == NEGATIVE)){
        x_->sign = NON_NEGATIVE;
        SUB(y_, x_, z);
        bigint_delete(&y_);
        bigint_delete(&x_);
        return;
    }

    if (x_->wordlen >= y_->wordlen){
        ADDC(x_, y_, z);
        bigint_delete(&y_);
        bigint_delete(&x_);
        return;
    }

    if (x_->wordlen < y_->wordlen){
        ADDC(y_, x_, z);
        bigint_delete(&y_);
        bigint_delete(&x_);
        return;
    } 
}

//SUBTRACT//
void SUB_AbB(word* A, word* B, unsigned int* b, word* C)
{
    unsigned int b_2 = 0;
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
    word* tmp = NULL;
    tmp = B->a;
    B->a = (word*)realloc(B->a, sizeof(word) * (A->wordlen));
    if (B->a == NULL) {
        printf("Memory Reallocation Fail");
        B->a = tmp;
        return;
    }
    for (int j = B->wordlen; j < A->wordlen; j++) {
        B->a[j] = 0;
    }

    unsigned int b = 0;                              
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
    bigint* x_ = NULL;
    bigint* y_ = NULL;

    bigint_assign(&x_, x);
    bigint_assign(&y_, y);

    if (IsZero(x_)){
        bigint_assign(z, y_);
        (*z)->sign = NEGATIVE; 
        bigint_delete(&y_);
        bigint_delete(&x_);
        return;
    }
    if (IsZero(y_)){
        bigint_assign(z, x_);
        (*z)->sign = NON_NEGATIVE; 
        bigint_delete(&y_);
        bigint_delete(&x_);
        return;
    }
    if(Compare(x_,y_)==0){
        bigint_set_zero(z);
        bigint_delete(&y_);
        bigint_delete(&x_);
        return;
    }
    if((y_->sign==NON_NEGATIVE)&&(x_->sign==NON_NEGATIVE)&&(Compare(x_,y_) != -1) )
    {
        SUBC(x_,y_,z);
        (*z)->sign = NON_NEGATIVE;
        bigint_delete(&y_);
        bigint_delete(&x_);
        return;
    }
    if((x_->sign == NON_NEGATIVE)&&(y_->sign==NON_NEGATIVE)&&(Compare(x_,y_)==-1))
    {
        SUBC(y_,x_,z);
        (*z)->sign = NEGATIVE;
        bigint_delete(&y_);
        bigint_delete(&x_);
        return;
    }
    if((x_->sign==NEGATIVE)&&(y_->sign==NEGATIVE)&&(Compare(x_,y_)!=-1))
    {   
        x_->sign = NON_NEGATIVE;
        y_->sign = NON_NEGATIVE;
        SUBC(y_,x_,z);
        bigint_delete(&y_);
        bigint_delete(&x_);
        return;
    }
    if((y_->sign==NEGATIVE)&&(x_->sign==NEGATIVE)&&(Compare(x_,y_)==-1))
    {  
        x_->sign = NON_NEGATIVE;
        y_->sign = NON_NEGATIVE;
        SUBC(x_,y_,z);
        (*z)->sign = NEGATIVE;
        bigint_delete(&y_);
        bigint_delete(&x_);
        return;
    }

    if((x_->sign == NON_NEGATIVE)&&(y_->sign == NEGATIVE))
    {
        y_->sign = NON_NEGATIVE;
        ADD(x_,y_,z);
        (*z)->sign = NON_NEGATIVE;
        bigint_delete(&y_);
        bigint_delete(&x_);
        return;
    }
    if((x_->sign == NEGATIVE)&&(y_->sign == NON_NEGATIVE))
    {   
        x_->sign = NON_NEGATIVE;
        ADD(x_,y_,z);
        (*z)->sign = NEGATIVE;        
        bigint_delete(&y_);
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
    word A0 = (*x) & (((unsigned long long)1 << (WordBitLen / 2)) - 1);
    
    word B1 = (*y) >> (WordBitLen / 2); 
    word B0 = (*y) & (((unsigned long long)1 << (WordBitLen / 2)) - 1);
    
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

    A1 = 0; A0 = 0; B1 = 0; B1 = 0;
    T1 = 0; T0 = 0; T = 0;
    C1 = 0; C0 = 0;
    bigint_assign(z, mul);
    bigint_delete(&mul);
}

void MULC_Naive(bigint* x, bigint* y, bigint** z)
{
    bigint* mul = NULL;
    bigint_create(&mul, (x->wordlen + y->wordlen));

    for (int i = 0; i < x->wordlen; i++){
        for (int j = 0; j < y->wordlen; j++){
            bigint* T = NULL;
            MUL_AB(&(x->a[i]), &(y->a[j]), &T);       
            LeftShift(T, WordBitLen * (i + j));
            ADD(mul, T, &mul);
            bigint_delete(&T);
        }
    }

    bigint_assign(z, mul);
    bigint_delete(&mul); 
}

void MULC_Karatsuba(bigint* x, bigint* y, bigint** z)
{   

    if (x->wordlen <= 4 || y->wordlen <= 4){
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
        S1->sign = NON_NEGATIVE; S0->sign = NON_NEGATIVE;
        MULC_Karatsuba(S1, S0, &S);
        if (S_sign == 0) S->sign = NON_NEGATIVE;
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
        
        bigint_delete(&R); bigint_delete(&S);
        bigint_delete(&S0); bigint_delete(&S1);
        bigint_delete(&T0); bigint_delete(&T1);
        bigint_delete(&B0); bigint_delete(&B1);
        bigint_delete(&A0); bigint_delete(&A1);
    }
}

void MUL(bigint* x, bigint* y, bigint** z)
{
    bigint* x_ = NULL;
    bigint* y_ = NULL;

    bigint_assign(&x_, x);
    bigint_assign(&y_, y);

    if (IsZero(x_) || IsZero(y_))
    {
        bigint* mul = NULL;
        bigint_create(&mul, 1);
        mul->a[0] = 0;
        bigint_assign(z, mul);

        bigint_delete(&mul);
        bigint_delete(&y_);
        bigint_delete(&x_);
        return;
    }
    if (IsOne(x_)){
        bigint_assign(z, y_);
        (*z)->sign = (x_->sign + y_->sign) % 2;
        
        bigint_delete(&y_);
        bigint_delete(&x_);
        return;
    }
    if (IsOne(y_)){
        bigint_assign(z, x_);
        (*z)->sign = (x_->sign + y_->sign) % 2;

        bigint_delete(&y_);
        bigint_delete(&x_);
        return;
    }
    int x_sign = x_->sign;
    int y_sign = y_->sign;
    x_->sign = NON_NEGATIVE;
    y_->sign = NON_NEGATIVE;
    MULC_Karatsuba(x_, y_, z);
    x_->sign = x_sign;
    y_->sign = y_sign;
    (*z)->sign = (x_->sign + y_->sign) % 2;

    bigint_delete(&y_);
    bigint_delete(&x_);
    return;
}

// DIVISION //
void Long_Division(bigint* A, bigint* B, bigint** Q)
{
    bigint* q = NULL;
    bigint* r = NULL;
    
    // Q Initial Setting 
    bigint_create(&q, 1);
    q->a[0] = 0;

    // R Initial Setting
    bigint_assign(&r, A);
    RightShift(r, WordBitLen);
    word A0 = A->a[0];

    for (int i = WordBitLen - 1; i >= 0; i--){
        if (r->a[0] >= ((unsigned long long)1<<(WordBitLen-1))){
            q->a[0] += ((unsigned long long)1<<i);
            LeftShift(r, 1);
            r->a[0] += (A0 >> i) & 0x01;
            SUB(r, B, &r);
        }    
        else{
            LeftShift(r, 1);
            r->a[0] += (A0 >> i) & 0x01;
            if (Compare(r, B) == 1 || Compare(r, B) == 0){
                q->a[0] += ((unsigned long long)1<<i);
                SUB(r, B, &r);
            }
        }
    }

    // show_bigint_hex(Q);
    // show_bigint_hex(R);

    bigint_assign(Q, q);
    bigint_delete(&r);
    bigint_delete(&q);
    A0 = 0;
}

void DIVCC(bigint* A, bigint* B, bigint** Q, bigint** R)
{
    bigint* q = NULL;
    bigint* r = NULL;
    bigint* Bq = NULL;

    if (A->wordlen == B->wordlen){
        bigint_create(&q, 1);
        q->a[0] = (word)(A->a[B->wordlen - 1] / B->a[B->wordlen - 1]);
    }

    if (A->wordlen == B->wordlen + 1){
        if (A->a[B->wordlen] == B->a[B->wordlen - 1]){
            bigint_create(&q, 1);
            #if WordBitLen == 32
            q->a[0] = 0xffffffff;
            #elif WordBitLen == 8
            q->a[0] = 0xff;
            #elif WordBitLen == 64
            q->a[0] = 0xffffffffffffffff;
            #endif
        } else {
            bigint* new_A = NULL;
            bigint* new_B = NULL;

            bigint_create(&new_A, 2);
            bigint_create(&new_B, 1);
            new_A->a[0] = A->a[B->wordlen - 1];
            new_A->a[1] = A->a[B->wordlen];
            new_B->a[0] = B->a[B->wordlen - 1];

            Long_Division(new_A, new_B, &q);
            
            bigint_delete(&new_A);
            bigint_delete(&new_B);
        }
    }

    MUL(B, q, &Bq);
    SUB(A, Bq, &r);

    bigint* t = NULL;
    bigint_create(&t, 1);
    t->a[0] = 1;

    while (r->sign == NEGATIVE){
        SUB(q, t, &q);
        ADD(r, B, &r);
    }

    bigint_assign(Q, q);
    bigint_assign(R, r);

    bigint_delete(&t);
    bigint_delete(&Bq);
    bigint_delete(&r);
    bigint_delete(&q);
}

void DIVC(bigint* A, bigint* B, bigint** Q, bigint** R)
{   
    bigint* q = NULL;
    bigint* r = NULL;
    bigint* A_ = NULL;
    bigint* B_ = NULL;

    bigint_assign(&A_, A);
    bigint_assign(&B_, B);

    if (Compare(A_, B_) == -1){
        bigint_create(&q, 1);
        q->a[0] = 0;

        bigint_assign(Q, q);
        bigint_assign(R, A_);

        bigint_delete(&q);
        bigint_delete(&B_);
        bigint_delete(&A_);
        return;
    }

    int k = 0;
    while(B_->a[B_->wordlen - 1] < ((unsigned long long)1 << (WordBitLen - 1))){
        LeftShift(A_, 1);
        LeftShift(B_, 1);
        k += 1;
    }

    DIVCC(A_, B_, &q, &r);

    bigint_assign(Q, q);
    RightShift(r, k);
    bigint_assign(R, r);

    bigint_delete(&B_);
    bigint_delete(&A_);
    bigint_delete(&r);
    bigint_delete(&q);
}

void DIV(bigint* A, bigint* B, bigint** Q, bigint** R)
{
    bigint* qi = NULL;
    bigint* r = NULL;
    bigint* q = NULL;

    if (Compare(A, B) == -1){
        bigint_create(&qi, 1);
        qi->a[0] = 0;

        bigint_assign(Q, qi);
        bigint_assign(R, A);

        bigint_delete(&qi);
        return;
    }

    if (IsZero(B)) {
        printf("\nDivision Zero Error\n");
        return;
    }

    bigint_create(&r, 1);
    r->a[0] = 0;

    bigint_create(&q, A->wordlen);

    for (int i = A->wordlen - 1; i >= 0; i--){
        LeftShift(r, WordBitLen);
        r->a[0] += A->a[i];
        DIVC(r, B, &qi, &r);
        q->a[i] = qi->a[0];
    }

    bigint_refine(q);
    bigint_refine(r);
    bigint_assign(Q, q);
    bigint_assign(R, r);

    bigint_delete(&q);
    bigint_delete(&r);
    bigint_delete(&qi);
}

void Single_percision_sqr(word A, bigint** C)
{
    bigint* c = NULL;
    bigint_create(&c, 2);

    int worddiv2 = (WordBitLen >> 1);
    word A1 = A >> worddiv2;
    word A0 = A % ((unsigned long long)1 << worddiv2);
    
    word C0 = A0 * A0;
    word C1 = A1 * A1;
    
    c->a[1] = C1;
    c->a[0] = C0;

    bigint* T = NULL;
    bigint_create(&T, 1);
    T->a[0] = A0 * A1;
    LeftShift(T, worddiv2 + 1);
    ADD(c, T, &c);

    bigint_assign(C, c);

    bigint_delete(&T);
    bigint_delete(&c);
}

void Sqr_Textbook(bigint* x, bigint** y)
{
    bigint* C1 = NULL; 
    bigint* C2 = NULL;
    bigint* T1 = NULL; 
    bigint* T2 = NULL;
    
    bigint_create(&C1, 1);
    bigint_create(&C2, 1);
    
    int t = x->wordlen;
    for (int i = 0; i < t; i++)
    {
        Single_percision_sqr(x->a[i], &T1);
        LeftShift(T1, 2 * i * WordBitLen);
        ADD(C1, T1, &C1);
        for (int j = i + 1; j < t; j++)
        {
            MUL_AB(&(x->a[i]), &(x->a[j]), &T2);
            LeftShift(T2, (i + j) * WordBitLen);
            ADD(C2, T2, &C2);
        }
    }  
    LeftShift(C2, 1);
    ADD(C1, C2, y);

    bigint_delete(&T2);
    bigint_delete(&T1);
    bigint_delete(&C2);
    bigint_delete(&C1);
}

void SQU(bigint* x, bigint** y)
{
    bigint* x_ = NULL;
    bigint_assign(&x_, x);

    if (IsZero(x_) || IsOne(x_)){
        bigint_assign(y, x_);
        (*y)->sign = NON_NEGATIVE;
        bigint_delete(&x_);
        return;
    }

    Sqr_karatsuba(x_, y);
    bigint_delete(&x_);
    return;
}

void Sqr_karatsuba(bigint* x, bigint** y)
{
    if (x->wordlen <= 4)
    {
        Sqr_Textbook(x, y);
        return;
    }
    
    int l = (x->wordlen + 1) >> 1;
    bigint* A1 = NULL;
    bigint* A0 = NULL;
    bigint_assign(&A0, x);
    bigint_assign(&A1, x);
    A0->sign = NON_NEGATIVE;
    A1->sign = NON_NEGATIVE;

    RightShift(A1, l * WordBitLen);
    Reduction(A0, l * WordBitLen);

    // printf("A1 : "); show_bigint_hex(A1);
    // printf("A0 : "); show_bigint_hex(A0);

    bigint* T0 = NULL; 
    bigint* T1 = NULL;
    Sqr_karatsuba(A1, &T1);
    Sqr_karatsuba(A0, &T0);

    // printf("T1 : "); show_bigint_hex(T1);
    // printf("T0 : "); show_bigint_hex(T0);

    bigint* R = NULL; 
    bigint* S = NULL;
    LeftShift(T1, 2 * l * WordBitLen);
    ADD(T1, T0, &R);
    // printf("R : "); show_bigint_hex(R);

    MULC_Karatsuba(A1, A0, &S);
    // printf("S : "); show_bigint_hex(S);

    LeftShift(S, l * WordBitLen +1);
    // printf("S : "); show_bigint_hex(S);
    
    ADDC(R, S, y);
    // printf("C : "); show_bigint_hex((*y));


    bigint_delete(&S);
    bigint_delete(&R);
    bigint_delete(&T1);
    bigint_delete(&T0);
    bigint_delete(&A0);
    bigint_delete(&A1);
}

void Exponentiation(bigint* x, bigint* N, bigint** z, bigint* M)
{
    if (IsZero(M)) {
        printf("Modular Zero Error");
        return;
    }
    if (IsOne(M)) {
        bigint* M_ = NULL;
        bigint_create(&M_, 1);
        M_->a[0] = 0;
        bigint_assign(z, M_);
        bigint_delete(&M_);
        return;
    }
    bigint* t0 = NULL;
    bigint* t1 = NULL;
    
    bigint_create(&t0, 1);
    bigint_assign(&t1, x);
    t0->a[0] = 1;

    bigint* N_ = NULL;
    bigint_assign(&N_, N);
    
    int l = 0;
    while (!IsZero(N_)) {
        l += 1;
        RightShift(N_, 1);
    }
    bigint_delete(&N_);
    
    for (int i = l - 1; i >= 0; i--) {
        bigint* Q1 = NULL;
        bigint* R1 = NULL;
        bigint* Q2 = NULL;
        bigint* R2 = NULL;
        if ((((N->a[i / WordBitLen]) >> (i % WordBitLen)) & 0x1) == 1)
        {
            MUL(t0, t1, &t0);
            DIV(t0, M, &Q1, &R1);
            bigint_assign(&t0, R1);
            SQU(t1, &t1);
            DIV(t1, M, &Q2, &R2);
            bigint_assign(&t1, R2);
        }
        else
        {
            MUL(t0, t1, &t1);
            DIV(t1, M, &Q1, &R1);
            bigint_assign(&t1, R1);
            SQU(t0, &t0);
            DIV(t0, M, &Q2, &R2);
            bigint_assign(&t0, R2);
        }
        bigint_delete(&R2);
        bigint_delete(&Q2);
        bigint_delete(&R1);
        bigint_delete(&Q1);
    }
    bigint_assign(z, t0);

    bigint_delete(&t1);
    bigint_delete(&t0);
}

void Exponentiation2(bigint* x, bigint* N, bigint** z, bigint* M)
{

    bigint* t0 = NULL;
    bigint* t1 = NULL;

    bigint_create(&t0, 1);
    bigint_assign(&t1, x);

    bigint* N_ = NULL;
    bigint_assign(&N_, N);
    int l = 0;
    while (!IsZero(N_)) {
        l += 1;
        RightShift(N_, 1);
    }
    bigint_delete(&N_);

    
    for (int i = l - 1; i >= 0; i--) {
        bigint* Q1 = NULL;
        bigint* R1 = NULL;
        bigint* Q2 = NULL;
        bigint* R2 = NULL;
        if ((((N->a[i / WordBitLen]) >> (i % WordBitLen)) & 0x1) == 0x1)
        {
            ADD(t0, t1, &t0);
            DIV(t0, M, &Q1, &R1);
            bigint_assign(&t0, R1);

            ADD(t1, t1, &t1);
            DIV(t1, M, &Q2, &R2);
            bigint_assign(&t1, R2);
        }
        else
        {
            ADD(t0, t1, &t1);
            DIV(t1, M, &Q1, &R1);
            bigint_assign(&t1, R1);

            ADD(t0, t0, &t0);
            DIV(t0, M, &Q2, &R2);
            bigint_assign(&t0, R2);
        }
        bigint_delete(&R2);
        bigint_delete(&Q2);
        bigint_delete(&R1);
        bigint_delete(&Q1);
    }

    bigint_assign(z, t0);

    bigint_delete(&t1);
    bigint_delete(&t0);
}