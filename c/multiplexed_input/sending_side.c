////////////////////////////////////////////////////
// make two named pipes and closes them if needed //
////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#define BUF_SIZE 100

int main()
{
    // random number
    int rnum;
    srand(time(NULL));
    
    char* fifo1_path = "./in1";
    char* fifo2_path = "./in2";
    
    int fifo1 = mkfifo(fifo1_path, 0666);
    int fifo2 = mkfifo(fifo2_path, 0666);

    if (fifo1 == -1 || fifo2 == -1) {
        perror("mkfifo");
        exit(EXIT_FAILURE);
    }

    int writefd1 = open(fifo1_path, O_WRONLY);
    int writefd2 = open(fifo2_path, O_WRONLY);

    if (writefd1 == -1 || writefd2 == -1) {
        perror("open call");
        exit(EXIT_FAILURE);
    }
    
    char buffer[BUF_SIZE] = "";
    while (1) {
        printf("Write input data: ");
        fgets(buffer, BUF_SIZE, stdin);

        // close both of named pipes
        if (!strcmp(buffer, "q")) {
            close(writefd1);
            close(writefd2);
        }

        // choose random pipe to write
        rnum = rand() % 2;
        switch(rnum) {
        case 0:
            write(writefd1, buffer, BUF_SIZE);
            printf("%s written to in1!\n", buffer);
            break;
        case 1:
            write(writefd2, buffer, BUF_SIZE);
            printf("%s written to in2!\n", buffer);
            break;
        }
    }
}
