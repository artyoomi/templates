//
// demonization programm
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void create_daemon()
{
    pid_t pid = fork();

    if (pid < 0) {
        perror("Error occured");
        exit(EXIT_FAILURE);
    }
    if (pid > 0)
        exit(EXIT_SUCCESS);
    if (pid == 0) {
        pid_t pid = setsid();
        if (pid < 0)
            exit(EXIT_FAILURE);
        
        printf("%d\n", pid);

        chdir("/");

        // close all 
        fclose(stdin);
        fclose(stdout);
        fclose(stderr);
    }
}

int main()
{
    create_daemon();

    while (1) {
        sleep(50);
        break;
    }

    return EXIT_SUCCESS;
}
