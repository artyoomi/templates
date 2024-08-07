#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <arpa/inet.h>

int
main(int argc, char *argv[])
{
    if (2 != argc) {
        fprintf(stderr, "Usage: %s <hostname>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int             i;
    const char     *hostname;
    struct hostent *host;
    char            ip_str[INET_ADDRSTRLEN];

    hostname = argv[1];
    host = gethostbyname(hostname);

    for (i = 0; (host->h_addr_list)[i] != NULL; i++) {
        inet_ntop(host->h_addrtype, (host->h_addr_list)[i],
                  ip_str, INET_ADDRSTRLEN);
        puts(ip_str);
    }
    
    return EXIT_SUCCESS;
}
