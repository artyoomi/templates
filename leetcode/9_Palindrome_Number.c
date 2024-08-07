/////////////////////////////////
// checks if num is palindrome //
/////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

bool is_palindrome(long x)
{
    long tmp_x = x;
    
    if (x < 0)
        tmp_x = -x;

    int reversed = 0, remainder;
    
    while (tmp_x != 0) {
        remainder = tmp_x % 10;
        reversed = reversed * 10 + remainder;
        tmp_x /= 10;
    }

    return reversed == x;
}

int main(int argc, char* argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Must be 2 arguments!\n");
        exit(EXIT_FAILURE);
    }
    
    printf("%d\n", is_palindrome(atoi(argv[1])));
}
