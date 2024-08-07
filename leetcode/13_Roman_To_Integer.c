/////////////////////////////////
// checks if num is palindrome //
/////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// returns -1 on error
int repres(char s) {
    switch(s) {
    case 'I':
        return 1;
    case 'V':
        return 5;
    case 'X':
        return 10;
    case 'L':
        return 50;
    case 'C':
        return 100;
    case 'D':
        return 500;
    case 'M':
        return 1000;
    default:
        fprintf(stderr, "[%c] must to be roman digit!\n", s);
        exit(EXIT_FAILURE);
    }
}

int romanToInt(char* s)
{
    int num = 0;
    size_t s_len = strlen(s);
    
    for (int i = 0; i < s_len; i++) {
        if (i + 1 < s_len && repres(s[i]) < repres(s[i + 1])) {
            num -= repres(s[i]);
        } else {
            num += repres(s[i]);
        }
    }

    return num;
}

int main(int argc, char* argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Must be 2 arguments!\n");
        exit(EXIT_FAILURE);
    }

    printf("Equivalent of roman number [%s] is number [%d]\n",
           argv[1], romanToInt(argv[1]));
}
