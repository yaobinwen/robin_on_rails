#include "conv_pid.h"
#include <errno.h>
#include <linux/netlink.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define MAX_PAYLOAD 128     /* maximum payload size */
#define BUFFER_SIZE 256     /* large enough buffer */

void usage(char * prog_name)
{
    printf("Usage: %s [netlink-pid]\n", prog_name);
    printf("If netlink-pid is not provided, `getpid()` is used.\n");
}

int main(int argc, char * argv[])
{
    if (argc > 2)
    {
        printf("Error: Too many arguments.\n");
        usage(argv[0]);
        return 1;
    }

    pid_t this_pid = getpid();

    if (2 == argc)
    {
        /* netlink-pid is provided. Then we use it. */
        /* Convert argv[1] into an `unsigned int`. */
        this_pid = strtoul(argv[1], NULL, 10);
    }

    printf("Starting recv (PID: %d)...\n", this_pid);

    /* Create a netlink socket. */
    int sock_fd = socket(AF_NETLINK, SOCK_DGRAM, NETLINK_ROUTE);

    /* Create my address. */
    struct sockaddr_nl my_addr;
    memset((void *)&my_addr, 0, sizeof(my_addr));
    my_addr.nl_family = AF_NETLINK;
    my_addr.nl_pid = this_pid;  /* self pid */
    my_addr.nl_groups = 0;  /* not in mcast groups */

    /* Bind my address to the socket. */
    bind(sock_fd, (struct sockaddr *)&my_addr, sizeof(my_addr));

    /* Prepare the netlink message buffer. */
    unsigned char buffer[BUFFER_SIZE];
    memset((void *)buffer, 0, BUFFER_SIZE);

    struct nlmsghdr * nlh = (struct nlmsghdr *)buffer;

    struct iovec iov;
    memset((void *)&iov, 0, sizeof(iov));
    iov.iov_base = (void *)buffer;  /* Must be the address of the buffer. */
    iov.iov_len = BUFFER_SIZE;      /* Must be the size of the buffer. */

    struct sockaddr_nl send_sa;
    struct msghdr msg;
    memset((void *)&msg, 0, sizeof(msg));
    msg.msg_name = &send_sa;
    msg.msg_namelen = sizeof(send_sa);
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;

    /* Read a message. */
    printf("Waiting for the message...\n");
    ssize_t ret = recvmsg(sock_fd, &msg, 0);
    if (ret > 0)
    {
        printf("Received message payload: %s\n", (char *)NLMSG_DATA(nlh));
    }
    else if (0 == ret)
    {
        printf("`recvmsg` returns 0.\n");
    }
    else
    {
        printf("Error: `recvmsg` fails. errno: %s\n", strerror(errno));
        return 1;
    }

    /* Close Netlink Socket */
    close(sock_fd);

    return 0;
}
