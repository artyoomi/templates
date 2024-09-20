#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#define SIZE 5120

void
udp_server(unsigned short port, const char *ip_str)
{
    int                udp_socket;
    char               *log_str;
    ssize_t            log_str_len;
    struct sockaddr_in local;

    log_str = (char *)malloc(SIZE * sizeof(char));
    
    udp_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (-1 == udp_socket) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    inet_aton("127.0.0.1", &local.sin_addr);
    local.sin_port = htons(port);
    local.sin_family = AF_INET;

    if (-1 == bind(udp_socket, (struct sockaddr *)&local, sizeof(local))) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    while (1) {
        log_str_len = read(udp_socket, log_str, SIZE);
        if (-1 == log_str_len) {
            perror("read");
            exit(EXIT_FAILURE);
        }
        log_str[log_str_len] = '\0';
        
        if (!strcmp(log_str, "OFF\n"))
            break;

        puts(log_str);
    }

    free(log_str);

    if (-1 == close(udp_socket)) {
        perror("close");
        exit(EXIT_FAILURE);
    }
}

int
main(int argc, char *argv[])
{
    if (2 != argc) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
    }

    const char     *ip_str = "127.0.0.1";
    unsigned short port = atoi(argv[1]);

    udp_server(port, ip_str);
        
    return EXIT_SUCCESS;
}
