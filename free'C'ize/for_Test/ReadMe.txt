Enterstellar Operation Test 
Enterstellar Operation Test with Python Embedding 

-How to use- 
You must set up Visual Studio to use python embedding code 
1. change your Solution Platform to x64
2. Project properties -> vc++ directories
3. Add python include folder path in include directories 
4. Add python libs folder path in library directories 

-Add Python file- 
Python file must be in the same location as the exe file 
<Debug mode>
	location : C:/EnterStellar/x64/Debug
<Release mode>
	location : C:/EnterStellar/x64/Debug 

-Testing- 
int main()
{
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
