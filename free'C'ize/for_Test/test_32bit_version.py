# -*- coding: utf-8 -*-
import math

def add(a, a_sign, b, b_sign, s, s_sign):
    bigint_a = int(a, 16) # a를 10진수 값으로 변환 
    bigint_b = int(b, 16) # b를 10진수 값으로 변환 
    
    if a_sign == 1:
        bigint_a *= -1
    
    if b_sign == 1:
        bigint_b *= -1

    bigint_sum = bigint_a + bigint_b # a와 b를 더하기 
    bigint_sum_sign = 0
    if bigint_sum < 0:
        bigint_sum_sign = 1

    sum = int(s, 16)
    if s_sign == 1:
        sum *= -1    

    if (hex(bigint_sum) == hex(sum)) and (bigint_sum_sign == s_sign): # 16진수 출력 값이 같은지 확인 
        return 1
    else:
        print("\nanswer : ", hex(bigint_sum))
        print("A : ", hex(bigint_a))
        print("A sign : ", a_sign)
        print("B : ", hex(bigint_b))
        print("B sign : ", b_sign)
        print("A + B : ", hex(sum))
        print("Sum sign : ", s_sign)
        return 0

def mul(a, b, m):
    bigint_a = int(a, 16) # a를 10진수 값으로 변환 
    bigint_b = int(b, 16) # b를 10진수 값으로 변환
    bigint_mul = bigint_a * bigint_b
    mul = hex(int(m, 16))
    if hex(bigint_mul) == mul: # 16진수 출력 값이 같은지 확인 
        return 1
    else:
        return 0

def sub(a, a_sign, b, b_sign, s, s_sign):
    bigint_a = int(a, 16) # a를 10진수 값으로 변환 
    if a_sign == 1:
        bigint_a *= -1

    bigint_b = int(b, 16) # b를 10진수 값으로 변환
    if b_sign == 1:
        bigint_b *= -1

    bigint_sub = bigint_a - bigint_b
    bigint_sub_sign = 0
    if bigint_sub < 0:
        bigint_sub_sign = 1
        
    sub = int(s, 16)
    if s_sign == 1:
        sub *= -1

    if (hex(bigint_sub) == hex(sub)) and (bigint_sub_sign == s_sign): # 16진수 출력 값이 같은지 확인 
        return 1
    else:
        print("\nanswer : ", hex(bigint_sub))
        print("A : ", hex(bigint_a))
        print("A sign : ", a_sign)
        print("B : ", hex(bigint_b))
        print("B sign : ", b_sign)
        print("A - B : ", hex(sub))
        print("Sum sign : ", s_sign)
        return 0

def get_sign(a) :
    if a < 0:
        return 1
    return 0

def bits(a):
    b = bin(a)
    b = b[2:]
    return len(b)

def Karatsuba(a, b):
    w = 32                          # 테스트할 때 마다 변경 필수!!!!!!!!!!!!!!!!!!!
    n = math.ceil(bits(a) / w)
    m = math.ceil(bits(b) / w)
    if 10 >= min(n, m):
        return (a * b)

    l = (max(n, m) + 1) >> 1
    lw = l * w
    a1, a0 = a >> lw , a % (2 ** lw)
    b1, b0 = b >> lw , b % (2 ** lw)
    
    t1, t0 = Karatsuba(a1, b1), Karatsuba(a0, b0)

    r = (t1 << (2*lw)) + t0

    s1, s0 = a0 - a1 , b1 - b0
    
    s = ((-1) ** (get_sign(s1) + get_sign(s0))) * Karatsuba(abs(s1), abs(s0))

    s = s + t1
    s = s + t0
    s = s << lw
    r = r + s
    return r

def mul_k(a, a_sign, b, b_sign, m, m_sign):
    bigint_a = 0
    bigint_b = 0

    bigint_a = int(a, 16)           # a는 C언어에서 전달한 bigint A 입니다.
    if a_sign == 1:
        bigint_a *= -1

    bigint_b = int(b, 16)           # b는 C언어에서 전달한 bigint B 입니다. 
    if b_sign == 1:
        bigint_b *= -1
                                    # m은 C언어에서 bigint A와 bigint B를 곱한 값입니다. 
    c1 = Karatsuba(bigint_a, bigint_b)  # 파이썬 카라추바 곱셈 값
    answer = bigint_a * bigint_b        # 정답
    if c1 == answer:
        c1_sign = 0
        if c1 <0:
            c1_sign = 1    # 파이썬 카루추바 값과 정답이 같다면
        
        m = int(m, 16)
        if m_sign == 1:
            m *= -1
        
        if hex(c1) == hex(m) and c1_sign == m_sign:        
            bigint_a = 0
            bigint_b = 0
            return 1
        else:
            print("\nC Error")
            print("A : ", hex(bigint_a))
            print("A sign : ", a_sign)
            print("B : ", hex(bigint_b))
            print("B sign : ", b_sign)
            print("M : " , hex(m))
            print('M sign : ', m_sign)
            print("Answer : ", hex(c1)) 
            
            bigint_a = 0
            bigint_b = 0
            return 0
    else:
        # 파이썬 카라추바 답과 실제 곱셈 값이 다른 경우 
        print("\nPython Error")
        bigint_a = 0
        bigint_b = 0
        return 0


def div(a, a_sign, b, b_sign, q, q_sign, r, r_sign):
    if (a_sign == 1 or b_sign == 1 or q_sign == 1 or r_sign == 1):
        print("Sign Error")
        return 0
    
    bigint_a = int(a, 16)
    bigint_b = int(b, 16)
    Q = hex(bigint_a // bigint_b)
    R = hex(bigint_a % bigint_b)
    if hex(int(q, 16)) == Q:
        if hex(int(r, 16)) == R:
            return 1
        else:
            print("나머지 Error!")
            return 0
    else:
        print("몫 Error!")
        return 0


def sqr(a, a_sign, b, b_sign):
    if b_sign == 1:
        print("Sign Error")
        return 0
    
    bigint_a = int(a, 16)
    if a_sign == 1:
        bigint_a *= -1
    
    s = bigint_a ** 2
    
    if hex(s) == hex(int(b, 16)):
        return 1
    else:
        print("Error!")
        print("A : ", bigint_a)
        print("Answer : ", hex(s))
        return 0


def Modular_exponentiation1(a, a_sign, n, n_sign, b, b_sign, m, m_sign):
    if m_sign == 1 or n_sign == 1 or a_sign == 1 or b_sign == 1:
        print("Sign Error")
        return 0
    
    bigint_a = int(a, 16)
    bigint_n = int(n, 16)
    bigint_m = int(m, 16)

    answer = 0
    answer = pow(bigint_a, bigint_n, bigint_m)
    if hex(answer) == hex(int(b, 16)):
        return 1
    else:
        print("\nAnswer = ", hex(answer))
        print("A = ", hex(bigint_a))
        print("N = ", hex(bigint_n))
        print("M = ", hex(bigint_m))
        print("My Answer = ", hex(int(b, 16)))
        return 0

def Modular_exponentiation2(a, a_sign, n, n_sign, b, b_sign, m, m_sign):
    if m_sign == 1 or n_sign == 1 or a_sign == 1 or b_sign == 1:
        print("Sign Error")
        return 0

    bigint_a = int(a, 16)
    bigint_n = int(n, 16)
    bigint_m = int(m, 16)

    answer = ((bigint_a) * (bigint_n)) % bigint_m
    if hex(answer) == hex(int(b, 16)):
        return 1
    else:
        print("\nAnswer = ", hex(answer))
        print("A = ", hex(bigint_a))
        print("N = ", hex(bigint_n))
        print("M = ", hex(bigint_m))
        print("My Answer = ", hex(int(b, 16)))
        return 0

# if __name__=="__main__":
#     mul_k("80", "50", 0)
