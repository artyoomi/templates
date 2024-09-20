/////////////////////////////////////////////////////
// this server get str with \0 terminating symbol, //
// sort symbols in str and send to client          //
/////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#define SIZE 5120
#define ehandler(msg) { perror(msg); exit(EXIT_FAILURE); }

int
comparator(const void *a, const void *b)
{
    const char c1 = *((char *)a);
    const char c2 = *((char *)b);
    
    if (c1 > c2)
        return -1;
    if (c1 < c2)
        return 1;
    return 0;
}

void
tcp_server(const char *ip_str, unsigned short port)
{
    int                listen_socket;
    int                connect_socket;
    struct sockaddr_in addr;
    char               *msg;
    ssize_t            msg_len;

    msg = (char *)malloc(SIZE * sizeof(char));

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_aton(ip_str, &addr.sin_addr);

    listen_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == listen_socket) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    if (-1 == bind(listen_socket, (struct sockaddr *)&addr, INET_ADDRSTRLEN)) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    if (-1 == listen(listen_socket, 5)) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    connect_socket = accept(listen_socket, NULL, NULL);
    if (-1 == connect_socket) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    while (1) {
        msg_len = recv(connect_socket, msg, SIZE, 0);

        if (-1 == msg_len) {
            perror("recv");
            exit(EXIT_FAILURE);
        }

        msg[msg_len] = '\0';

        if (!strcmp(msg, "OFF\n"))
            break;

        qsort(msg, msg_len, sizeof(char), comparator);

        if (-1 == send(connect_socket, msg, msg_len, 0)) {
            perror("send");
            exit(EXIT_FAILURE);
        }
    }

    if (-1 == close(listen_socket))
        ehandler("close");
    if (-1 == close(connect_socket))
        ehandler("close");
}

int
main(int argc, char *argv[])
{
    if (2 != argc) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        return EXIT_SUCCESS;
    }

    const char     *ip_str = "127.0.0.1";
    unsigned short port = atoi(argv[1]);

    tcp_server(ip_str, port);
    
    return EXIT_SUCCESS;
}
