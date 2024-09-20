#include <stdio.h>

int
mySqrt(long x)
{
    if (x <= 0)
        return 0;
    
    for (int i = 1; i < x + 1; i++) {
        if (i * i > x)
            return i - 1;
    }

    return 1;
}

int
main()
{
    long num;
    scanf("%ld", &num);
    
    printf("Square root of %ld (floor rounded) is %d\n",
           num, mySqrt(num));
    
    return 0;
}
