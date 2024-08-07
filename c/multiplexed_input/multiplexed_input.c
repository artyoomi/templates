//
//
//

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/select.h>

int main()
{
    int sum = 0;
    
    int readfd1 = open("./in1", O_RDONLY);
    int readfd2 = open("./in2", O_RDONLY);
    if (readfd1 == -1 || readfd2 == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    fd_set read_set;
    FD_ZERO(&read_set);
    FD_SET(readfd1, &read_set);
    FD_SET(readfd2, &read_set);

    char input[100] = "";
    while (1) {
        if (select(readfd2 + 1, &read_set, NULL, NULL, NULL) == -1) {
            perror("select");
            exit(EXIT_FAILURE);
        }

        for (int i = readfd1; i < readfd2 + 1; i++) {
            if (FD_ISSET(i, &read_set)) {
                read(i, input, sizeof(input));
                sum += atoi(input);
            }
        }
    }
    
    return EXIT_SUCCESS;
}
