#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BLOCK 15

// args: line to split / str with delims / array to store elements
// returns: count of splitted elements
size_t split_line_to_ints(char* line, char* delims, int** ints)
{
    size_t count = 0;

    *ints = (int*)malloc(BLOCK * sizeof(int));
    
    size_t alloc_size = BLOCK;
    
    char* context = NULL;
    char* token = strtok_r(line, delims, &context);
    while (token != NULL) {
        if (count == alloc_size) {
            int* new_ints = (int*)realloc(*ints, sizeof(int) * (alloc_size + BLOCK));
            if (!new_ints) {
                perror("new_ints");
                exit(EXIT_SUCCESS);
            }
            
            *ints = new_ints;
            alloc_size += BLOCK;
        }
        
        (*ints)[count++] = atoi(token);
        token = strtok_r(NULL, delims, &context);
    }

    free(line);
    return count;
}
