///////////////////////////////////////////
// sum nums from to shared regions       //
// and store in user added shared region //
///////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>

#define REG_SIZE 1000
#define COUNT 100

int main(int argc, char* argv[])
{
    int*  arr1;
    int*  arr2;
    int*  arr3;
    
    int   shmid1;
    int   shmid2;
    int   shmid;
    key_t key;
    
    if (argc != 3) {
        fprintf(stderr, "Two arguments must be passed to the program!\n");
        return EXIT_SUCCESS;
    }

    shmid1 = shmget(atoll(argv[1]), REG_SIZE, 0);
    shmid2 = shmget(atoll(argv[2]), REG_SIZE, 0);

    // attach to shared memory regions
    arr1 = (int*)shmat(shmid1, NULL, SHM_RDONLY);
    if ((void *)-1 == arr1) {
        perror("arr1 shmat");
        exit(EXIT_FAILURE);
    }

    arr2 = (int*)shmat(shmid2, NULL, SHM_RDONLY);
    if ((void*)-1 == arr2) {
        perror("arr2 shmat");
        exit(EXIT_FAILURE);
    }

    // get shm key
    key = ftok(".", 'a');
    if (-1 == key) {
        perror("ftok");
        exit(EXIT_FAILURE);
    }

    // create new shared region
    shmid = shmget(key, REG_SIZE, 0666 | IPC_CREAT);
    if (-1 == shmid) {
        perror("arr2 shmget");
        exit(EXIT_FAILURE);
    }
    
    arr3 = (int*)shmat(shmid, NULL, 0);
    if ((void*)-1 == arr3) {
        perror("arr3 shmat");
        exit(EXIT_FAILURE);
    }

    size_t i;
    for (i = 0; i < COUNT; i++)
        arr3[i] = arr1[i] + arr2[i];

    printf("%d\n", key);
    
    if (shmdt((void*)arr1) || shmdt((void*)arr2) || shmdt((void*)arr3)) {
        perror("shmdt");
        exit(EXIT_FAILURE);
    }
    
    return EXIT_SUCCESS;
}
