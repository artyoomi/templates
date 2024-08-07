///////////////////////////////////////////////////////////////////
// this program executes utility with some argument and counts   //
// the number of '0' symbols in utility's output                 //
///////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// returns count of zeros in line
size_t line_zeros_count(char * lineptr)
{
    size_t count = 0;
    
    int i;
    for (i = 0; i < strlen(lineptr); i++) {
        if ('0' == lineptr[i])
            count++;
    }

    return count;
}

// counts and returns amount of '0' symbols in file
size_t file_zeros_count(FILE * fd)
{
    size_t count = 0;
    
    char buffer[255] = "";
    while (fgets(buffer, 255, fd))
        count += line_zeros_count(buffer);

    return count;
}

int main(int argc, char * argv[])
{
    if (argc != 3) {
        fprintf(stderr, "To few or to many arguments!\n");
        return EXIT_SUCCESS;
    }

    // size of strings + space + \0
    const size_t cmd_len = strlen(argv[1]) + strlen(argv[2]) + 2;
    char * cmd = (char*)malloc(cmd_len * sizeof(char));
    snprintf(cmd, cmd_len, "%s %s", argv[1], argv[2]);
    
    if (!cmd) {
        perror("cmd string");
        exit(EXIT_FAILURE);
    }
    
    FILE * fd = popen(cmd, "r");
    if (!fd) {
        perror("cmd popen");
        exit(EXIT_FAILURE);
    }

    size_t count = file_zeros_count(fd);
    printf("%ld\n", count);

    free(cmd);
    pclose(fd);

    return EXIT_SUCCESS;
}
