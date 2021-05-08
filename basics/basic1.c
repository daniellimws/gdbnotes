#include <stdio.h>

int goo(int a)
{
    return a * a;
}

int foo(int a)
{
    return goo(a + 1);
}

int main()
{
    // comment
    int a = 10;
    int b = goo(a);
    
    if (b == 1337)
        puts("How did you get here?");
    else
        puts("Program ended");

    return 0;
}
