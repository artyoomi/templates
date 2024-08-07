////////////////////////////////////////////////////////////////
// finds the number of all child processes of a given process //
// (including this one)                                       //
////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

// len of "pgrep -P <PID>\0"
// max len of PID will be 20
#define CMD_LEN 30
#define MPID_LEN 20 // max pid string len

#define remove_sn(str, len) \
        ({if (len >= 1 && str[len - 1] == '\n') str[len - 1] = '\0';})

size_t children_count(pid_t pid)
{
    static size_t count = 0;
    
    char * cmd = (char*)malloc(CMD_LEN * sizeof(char));
    snprintf(cmd, CMD_LEN, "pgrep -P %d", pid);
    
    FILE * fp = popen(cmd, "r");
    if (!fp) {
        perror(cmd);
        free(cmd);
        exit(EXIT_FAILURE);
    }

    char lineptr[MPID_LEN] = "";
    while (fgets(lineptr, MPID_LEN, fp)) {
        remove_sn(lineptr, strlen(lineptr));

        children_count(atoi(lineptr));
    }
    count++;

    pclose(fp);
    free(cmd);

    return count;
}

int main(int argc, char** argv)
{
    if (argc != 2) {
        puts("Too many or few arguments, must be ./a.out <PID>");
        return EXIT_FAILURE;
    }

    pid_t pid = atoi(argv[1]);

    printf("%ld\n", children_count(pid));
    
    return EXIT_SUCCESS;    
}
