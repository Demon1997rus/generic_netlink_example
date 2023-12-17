#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <asm/types.h>
#include <linux/netlink.h>
#include <linux/socket.h>
#include <errno.h>

#define MAX_PAYLOAD 1024

int fd;
sockaddr_nl src_addr;
sockaddr_nl dest_addr;
nlmsghdr* nlh = nullptr;
iovec iov;
msghdr msg;

int main()
{
    fd = socket(AF_NETLINK, SOCK_RAW, NETLINK_GENERIC);
    if (fd < 0)
    {
        std::cout << "Create socket failed";
        return -1;
    }

    memset(&src_addr, 0, sizeof(src_addr));
    src_addr.nl_family = AF_NETLINK;
    src_addr.nl_pid = 2;
    src_addr.nl_groups = 0;

    if (bind(fd, reinterpret_cast<sockaddr*>(&src_addr), sizeof(src_addr)) < 0)
    {
        std::cout << "Binding socket failed";
        return -1;
    }

    nlh = reinterpret_cast<nlmsghdr*>(malloc(NLMSG_SPACE(MAX_PAYLOAD)));
    memset(nlh, 0, NLMSG_SPACE(MAX_PAYLOAD));
    nlh->nlmsg_len = NLMSG_SPACE(MAX_PAYLOAD);
    nlh->nlmsg_pid = 1000;
    nlh->nlmsg_flags = 0;

    iov.iov_base = &nlh;
    iov.iov_len = nlh->nlmsg_len;

    memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.nl_family = AF_NETLINK;
    dest_addr.nl_groups = 0;
    dest_addr.nl_pid = 1;

    msg.msg_name = &dest_addr;
    msg.msg_namelen = sizeof(dest_addr);
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;

    while (true)
    {
        recvmsg(fd, &msg, 0);
        std::cout << "Received message:" << reinterpret_cast<char*>(NLMSG_DATA(nlh));
    }
    close(fd);
    return 0;
}
