/*
 * Program logic:
 * 1. Create shared memory region;
 * 2. Write "Hello world!" in this region;
 * 3. Print to stdout shared memory key or
 * error code and message (if occured).
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "macro.h"

// String to output in shared memory segment
#define OUTPUT_STR  "Hello world!"

int main()
{
        key_t shm_key;
        int   shmid;
        char* segm_seq;
        
        // Generate shared memory key
        if ((shm_key = ftok("./shm_reader", 'x')) == -1) {
                PRINT_ERRNO_MSG("ftok");
                exit(EXIT_FAILURE);
        }

        // Create shared memory segment
        if ((shmid = shmget(shm_key, REGION_SIZE, IPC_CREAT | 0600)) == -1) {
                PRINT_ERRNO_MSG("shmget");
                exit(EXIT_FAILURE);
        }

        // Attach segm_seq to shared memory segment
        if ((segm_seq = (char*)shmat(shmid, NULL, 0)) == (void*) -1) {
                PRINT_ERRNO_MSG("shmat");
                shmctl(shmid, IPC_RMID, NULL);
                exit(EXIT_FAILURE);
        }

        // Write OUTPUT_STR to shared memory region
        strncpy(segm_seq, OUTPUT_STR, REGION_SIZE);

        // Detach from shared memory region
        if (shmdt(segm_seq) == -1) {
                PRINT_ERRNO_MSG("shmdt");
                shmctl(shmid, IPC_RMID, NULL);
                exit(EXIT_FAILURE);
        }

        // Write shared memory region key to stdout
        printf("%d\n", shm_key);
        
        return 0;
}
