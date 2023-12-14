#include "clientinterface.h"

#include "clientinterfaceprivate.h"

ClientInterface::ClientInterface() : d(new ClientInterfacePrivate) {}

ClientInterface::~ClientInterface()
{
    if (close(d->nl_fd) < 0)
    {
        qCritical() << Q_FUNC_INFO;
        qCritical() << "Error code:" << errno;
    }
}

bool ClientInterface::open()
{
    bool result;
    result = d->openSocket();
    result = d->bindSocket();
    return result;
}

int ClientInterface::write(const QByteArray& data) { return 1; }

/*!
 * \brief ClientInterfacePrivate::openSocket - Open the socket. Note that protocol = NETLINK_GENERIC
 */
bool ClientInterfacePrivate::openSocket()
{
    nl_fd = socket(AF_NETLINK, SOCK_RAW, NETLINK_GENERIC);
    if (nl_fd < 0)
    {
        qCritical() << Q_FUNC_INFO;
        qCritical() << "Socket opening failed";
        return false;
    }
    return true;
}

/*!
 * \brief ClientInterfacePrivate::bindSocket - Bind the socket.
 */
bool ClientInterfacePrivate::bindSocket()
{
    ::bzero(&nl_address, sizeof(nl_address));
    nl_address.nl_family = AF_NETLINK;
    nl_address.nl_groups = 0;
    nl_address.nl_pid = 10000;
    if (bind(nl_fd, reinterpret_cast<sockaddr*>(&nl_address), sizeof(nl_address)) < 0)
    {
        qCritical() << Q_FUNC_INFO;
        qCritical() << "Error in binding";
        return false;
    }
    return true;
}
