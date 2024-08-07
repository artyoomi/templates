#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    int optchar;
    const char optstr[] = "q:iv";

    struct option opts[] = {
        {"query", required_argument, NULL, 'q'},
        {"longinformationrequest", no_argument, NULL, 'i'},
        {"version", no_argument, NULL, 'v'},
        {0, 0, 0, 0}
    };

    while (-1 != (optchar = getopt_long(argc, argv, optstr, opts, &optind))) {
        switch (optchar) {
        case 'q':
            break;
        case 'i':
            break;
        case 'v':
            break;
        default:
            printf("-\n");
            return 0;
        }
    }
    printf("+\n");
    
    return EXIT_SUCCESS;
}
