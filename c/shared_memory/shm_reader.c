/*
 * Program logic:
 * 1. Input key of shared memory from stdout of first program;
 * 2. Read data from shared memory region;
 * 3. Print recieved data to stdout or error code with message;
 * 4. Delete shared memory region for all processes.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "macro.h"

int main()
{
        key_t shm_key;
        int   shmid;
        char* segm_seq;
        char  recieved_data[REGION_SIZE];

        int ret = 0;
        // Get data from stdout of first program
        if ((ret = scanf("%d", &shm_key)) != 1) {
                PRINT_ERRMSG("scanf", ret, "Not enough values in input");
                exit(EXIT_FAILURE);
        }
        
        // Get shared memory segment id
        if ((shmid = shmget(shm_key, REGION_SIZE, 0600)) == -1) {
                PRINT_ERRNO_MSG("shmget");
                exit(EXIT_FAILURE);
        }

        // Attach segm_seq to shared memory segment
        if ((segm_seq = (char*)shmat(shmid, NULL, SHM_RDONLY)) == (void*) -1) {
                PRINT_ERRNO_MSG("shmat");
                shmctl(shmid, IPC_RMID, NULL);
                exit(EXIT_FAILURE);
        }

        // Read data from shared memory region
        strncpy(recieved_data, segm_seq, REGION_SIZE);

        // Detach from shared memory region
        if (shmdt(segm_seq) == -1) {
                PRINT_ERRNO_MSG("shmdt");
                shmctl(shmid, IPC_RMID, NULL);
                exit(EXIT_FAILURE);
        }

        // Delete shared memory region
        if (shmctl(shmid, IPC_RMID, NULL) == -1) {
                PRINT_ERRNO_MSG("shmctl");
                exit(EXIT_FAILURE);
        }

        // Write data from shared memory region to stdout
        printf("%s\n", recieved_data);
        
        return 0;
}
