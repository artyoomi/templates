///////////////////////////////////
// prints all ips of domain name //
///////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>

// ipv4 and ipv6
struct addrinfo*
get_ips(const char *hostname)
{
    int             status;
    struct addrinfo hints;
    struct addrinfo *list;
    struct addrinfo *list_head;
    
    memset(&hints, 0, sizeof(hints));   // reset to zero all values of struct
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    status = getaddrinfo(hostname, NULL, &hints, &list);
    if (0 != status) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        exit(EXIT_FAILURE);
    }

    return list;
}

// print all IPv4 and IPv6 addresses
void
print_ips(struct addrinfo *list)
{
    struct addrinfo *tmp;
    char            ipstr[INET6_ADDRSTRLEN];
    
    printf("IP addresses:\n\n");

    for (tmp = list; tmp != NULL; tmp = tmp->ai_next) {
        void *addr;
        char *ipver;

        // get the pointer to the address itself
        // different fields in IPv4 and IPv6
        if (AF_INET == tmp->ai_family) {
            struct sockaddr_in *ipv4 = (struct sockaddr_in *)tmp->ai_addr;
            addr = &(ipv4->sin_addr);
            ipver = "IPv4";
        } else {
            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)tmp->ai_addr;
            addr = &(ipv6->sin6_addr);
            ipver = "IPv6";
        }

        inet_ntop(tmp->ai_family, addr, ipstr, sizeof ipstr);
        printf("\t%s: %s\n", ipver, ipstr);
    }
}

// ipv4 only
void
print_unique_ips(struct addrinfo *list)
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

    struct addrinfo *list;

    list = get_ips(argv[1]);

    print_ips(list);

    return EXIT_SUCCESS;
}
