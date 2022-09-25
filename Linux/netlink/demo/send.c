#include "conv_pid.h"
#include <errno.h>
#include <linux/netlink.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define MAX_PAYLOAD 128    /* maximum payload size */
#define BUFFER_SIZE 256    /* large enough buffer */

void usage(char * prog_name)
{
    printf("Usage: %s <recv-pid> <message>\n", prog_name);
}

int main(int argc, char * argv[])
{
    if (argc < 3)
    {
        printf("Error: Not enough arguments.\n");
        usage(argv[0]);
        return 1;
    }

    if (argc > 3)
    {
        printf("Error: Too many arguments.\n");
        usage(argv[0]);
        return 1;
    }

    char * pid_str = argv[1];
    char * msg_str = argv[2];
    size_t msg_len = strlen(msg_str);
    if (msg_len > MAX_PAYLOAD)
    {
        printf("Error: Too long message (at most %d chars).\n", MAX_PAYLOAD);
        return 1;
    }

    /* Convert argv[1] into an `unsigned int`. */
    unsigned int recv_pid = strtoul(argv[1], NULL, 10);

    /* Create a netlink socket. */
    int sock_fd = socket(AF_NETLINK, SOCK_RAW, NETLINK_ROUTE);

    /* Create my address. */
    struct sockaddr_nl my_addr;
    memset((void *)&my_addr, 0, sizeof(my_addr));
    my_addr.nl_family = AF_NETLINK;
    my_addr.nl_pid = getpid();  /* self pid */
    my_addr.nl_groups = 0;  /* not in mcast groups */

    /* Bind my address to the socket. */
    bind(sock_fd, (struct sockaddr *)&my_addr, sizeof(my_addr));

    /* Create the destination's address. */
    struct sockaddr_nl dest_addr;
    memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.nl_family = AF_NETLINK;
    dest_addr.nl_pid = recv_pid;
    dest_addr.nl_groups = 0; /* unicast */

    /* Prepare the netlink message buffer. */
    unsigned char buffer[BUFFER_SIZE];
    memset((void *)buffer, 0, BUFFER_SIZE);

    struct nlmsghdr * nlh = (struct nlmsghdr *)buffer;

    /* Fill the netlink message header. */
    nlh->nlmsg_len = NLMSG_LENGTH(msg_len);
    nlh->nlmsg_pid = my_addr.nl_pid;
    nlh->nlmsg_flags = 0;
    /* Fill in the netlink message payload */
    strcpy(NLMSG_DATA(nlh), msg_str);

    struct iovec iov;
    memset((void *)&iov, 0, sizeof(iov));
    iov.iov_base = (void *)nlh;
    iov.iov_len = nlh->nlmsg_len;

    struct msghdr msg;
    memset((void *)&msg, 0, sizeof(msg));
    msg.msg_name = (void *)&dest_addr;
    msg.msg_namelen = sizeof(dest_addr);
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;

    /* Send the message. */
    printf("Sending a message to the process %d...\n", recv_pid);
    ssize_t ret = sendmsg(sock_fd, &msg, 0);
    if ((ssize_t)-1 == ret)
    {
        printf("Error: `sendmsg` fails. errno: %s\n", strerror(errno));
        return 1;
    }

    /* Close Netlink Socket */
    close(sock_fd);

    return 0;
}
