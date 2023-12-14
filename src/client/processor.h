#pragma once

#include <QObject>

#include "../interface-lib/clientinterface.h"

class Processor : public QObject
{
    Q_OBJECT
public:
    explicit Processor(QObject* parent = nullptr);

private:
    QScopedPointer<AbstractInterface> interface;
};
