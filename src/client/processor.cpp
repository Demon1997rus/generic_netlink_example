#include "processor.h"

#include <QDebug>

Processor::Processor(QObject* parent) : QObject(parent), interface(new ClientInterface)
{
    if (!interface->open())
    {
        qFatal("Interface opening failed %s", Q_FUNC_INFO);
    }
}
