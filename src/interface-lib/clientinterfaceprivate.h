#pragma once

#include <QDebug>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <poll.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <signal.h>
#include <linux/genetlink.h>

class ClientInterfacePrivate
{
public:
    bool openSocket();
    bool bindSocket();

public:
    int nl_fd = -1;          // файловый дескриптор сокета netlink
    sockaddr_nl nl_address;  // адрес сокета netlink
};
