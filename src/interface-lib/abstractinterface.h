#pragma once

#include <QByteArray>

class AbstractInterface
{
public:
    virtual ~AbstractInterface() {}
    virtual bool open() = 0;
    virtual int write(const QByteArray& data) = 0;
};
