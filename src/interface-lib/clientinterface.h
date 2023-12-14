#pragma once

#include <QScopedPointer>

#include "abstractinterface.h"

class ClientInterfacePrivate;
class ClientInterface : public AbstractInterface
{
public:
    ClientInterface();
    ~ClientInterface();

    // AbstractInterface interface
public:
    bool open() override;
    int write(const QByteArray& data) override;

private:
    Q_DISABLE_COPY(ClientInterface)
    QScopedPointer<ClientInterfacePrivate> d;
};
