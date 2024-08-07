// outputs the path from this process to a process
// with a PID equal to 1

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define MAX_PATH 4096

pid_t get_ppid(pid_t pid)
{
    // allocate memory to max size of path in linux 
    char* path = (char*)malloc(MAX_PATH * sizeof(char));
    snprintf(path, MAX_PATH, "/proc/%d/stat", pid);

    FILE* fd = fopen(path, "r");
    if (!fd) {
        perror(path);
        exit(EXIT_FAILURE);
    }

    long tgid = 0;
    char name[50] = "";
    char state[10] = "";
    pid_t ppid = 0;
    
    fscanf(fd, "%ld ", &tgid);
    fscanf(fd, "(%s) ", name);
    fscanf(fd, "%s ", state);
    fscanf(fd, "%d", &ppid);

    return ppid;

    fclose(fd);
    free(path); 
}

int main(int argc, char** argv)
{
    if (argc != 2) {
        puts("Too many or few arguments, must be ./a.out <PID>");
        return EXIT_FAILURE;
    }

    pid_t pid = atoi(argv[1]);

    printf("%d\n", pid);
    while (pid != 1) {
        pid = get_ppid(pid);
        printf("%d\n", pid);
    }

    return EXIT_SUCCESS;    
}
