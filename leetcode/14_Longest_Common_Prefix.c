#include <stdio.h>
#include <stdlib.h>

#define SIZE 201

char* longestCommonPrefix(char** strs, int strsSize)
{
    char prefix[SIZE] = "";

    // iterate throw strs
    for (int i = 0; i < strsSize; i++) {
        // iterate throw 
        for (int j = 0; j < SIZE; j++) {
            if ()
        }
    }
    
    return "JOKE";
}

int main(int argc, char* argv[])
{
    if (argc < 2) {
        printf("Must be at least 2 arguments!\n");
        return EXIT_SUCCESS;
    }

    printf("Longest common prefix is [%s]\n",
           longestCommonPrefix(&argv[1], argc - 1));
}
