#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define MAX_PATH 4096

int main()
{
    pid_t pid = getpid();
        
    // allocate memory to max size of path in linux 
    char* path = (char*)malloc(MAX_PATH * sizeof(char));
    snprintf(path, MAX_PATH, "/proc/%d/stat", pid);

    FILE* fd = fopen(path, "r");

    long tgid = 0;
    char name[50] = "";
    char state[10] = "";
    pid_t ppid = 0;
    
    //fscanf(fd, "%ld (%s)%c %d", &tgid, name, &state, &ppid);
    fscanf(fd, "%ld ", &tgid);
    fscanf(fd, "(%s) ", name);
    fscanf(fd, "%s ", state);
    fscanf(fd, "%d", &ppid);

    printf("%d\n", ppid);

    fclose(fd);
}
