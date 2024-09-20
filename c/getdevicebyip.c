#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <ifaddrs.h>

int
main(int argc, char *argv[])
{
	struct ifaddrs *addrs, *tmp;
	struct sockaddr_in *sa;
	char buf[32];
    
	if (-1 == getifaddrs(&addrs)) {
		perror("getifaddrs");
		exit(EXIT_FAILURE);
	}

	for (tmp = addrs; tmp != NULL; tmp = tmp->ifa_next) {
		if (tmp->ifa_addr && (tmp->ifa_flags & IFF_UP) && tmp->ifa_addr->sa_family == AF_INET) {
			sa = (struct sockaddr_in *)(tmp->ifa_addr);
			inet_ntop(tmp->ifa_addr->sa_family, (void *)&(sa->sin_addr), buf, sizeof(buf));
			if (!strcmp(argv[1], buf)) {
				printf("%s\n", tmp->ifa_name);
			}
		}
	}
  
	freeifaddrs(addrs);
	
	return 0;
}
