///////////////////////////////////
// prints all ips of domain name //
///////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>

struct addrinfo*
get_ips(const char *hostname)
{
    int             status;
    struct addrinfo hint;
    struct addrinfo *list;
    struct addrinfo *list_head;
    
    memset(&hint, 0, sizeof(hint));   // reset to zero all values of struct
    hint.ai_family = AF_INET;         // allows IPv4 only

    status = getaddrinfo(hostname, NULL, &hint, &list);
    if (0 != status) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        exit(EXIT_FAILURE);
    }

    list_head = list;
    while (NULL != list_head->ai_next) {
        // printf("struct addrinfo info:\n");
        // printf("int ai_flags: %d\n", list_head->ai_flags);
        // printf("int ai_family: %d\n", list_head->ai_family);
        // printf("int ai_socktype: %d\n", list_head->ai_socktype);
        // printf("int ai_protocol: %d\n", list_head->ai_protocol);
        // printf("socklen_t ai_addrlen: %u\n", list_head->ai_addrlen);
        // printf("struct sockaddr *ai_addr info:\n");
        // printf("\tsa_family_t sa_family: %u\n", list_head->ai_addr->sa_family);
        // printf("\tchar sa_data[]: %s\n", list_head->ai_addr->sa_data);
       
        list_head = list_head->ai_next;
    }

    return list;
}

void
print_unique_ip(struct addrinfo *list)
{
    struct addrinfo *curr;
    struct in_addr  ip_addr;
    char            curr_ip_str[INET_ADDRSTRLEN] = "";
    char            prev_ip_str[INET_ADDRSTRLEN] = "";
    
    curr = list;

    while (NULL != curr->ai_next) {
        ip_addr = ((struct sockaddr_in *)(curr->ai_addr))->sin_addr;
        inet_ntop(curr->ai_family, &ip_addr, curr_ip_str, INET_ADDRSTRLEN);

        if (strcmp(curr_ip_str, prev_ip_str))
            puts(curr_ip_str);

        curr = curr->ai_next;

        strcpy(prev_ip_str, curr_ip_str);
    }
}

int
main(int argc, char *argv[])
{
    if (2 != argc) {
        fprintf(stderr, "Usage: %s <hostname>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char      *hostname;
    struct addrinfo *list;

    hostname = argv[1];
    list = get_ips(hostname);

    print_unique_ip(list);

    return EXIT_SUCCESS;
}
