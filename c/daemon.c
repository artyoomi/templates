//////////////////////////////////////////
// simple daemon with sighal processing //
//////////////////////////////////////////

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

void sigurg_handler(int signalno)
{
    exit(EXIT_SUCCESS);
}

int main()
{
    pid_t pid = fork();

    // error
    if (-1 == pid) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    // child process
    if (0 == pid) {
        // change working directory to "/"
        if (chdir("/")) {
            perror("chdir");
            exit(EXIT_FAILURE);
        }
        // set new session id
        pid_t sid = setsid();
        if (sid == -1) {
            perror("setsid");
            exit(EXIT_FAILURE);
        }
        printf("%d\n", sid);
        
        // close all I/O files
        fclose(stdin);
        fclose(stdout);
        fclose(stderr);

        // setup signal handler
        signal(SIGURG, sigurg_handler);

        while (1);
    // parent process
    } else {
        exit(EXIT_SUCCESS);
    }
}
