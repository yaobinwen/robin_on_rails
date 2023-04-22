/**
 * This program is used to learn what the function `getaddrinfo()` does. The
 * code refers to https://jameshfisher.com/2018/02/03/what-does-getaddrinfo-do/
 * with slight modification.
 */

#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>


void usage(char * prog) {
    fprintf(stdout, "Usage: %s HOSTNAME\n", prog);
}


int main(int argc, char * argv[]) {
    char * prog = argv[0];
    char * hostname = NULL;
    struct addrinfo* addr = NULL;

    if (argc < 2) {
        fprintf(stderr, "Error: not enough arguments.\n");
        usage(prog);
        return 1;
    }

    if (argc > 2) {
        fprintf(stderr, "Error: too many arguments.\n");
        usage(prog);
        return 1;
    }

    hostname = argv[1];

    int ret = getaddrinfo(hostname, NULL, NULL, &addr);
    if (ret != 0) {
        fprintf(stderr, "Error: %s\n", gai_strerror(ret));
        return 1;
    }

    struct sockaddr_in* internet_addr = (struct sockaddr_in*)addr->ai_addr;
    fprintf(
        stdout,
        "'%s' is at '%s'\n",
        hostname,
        inet_ntoa(internet_addr->sin_addr)
    );

    return 0;
}
