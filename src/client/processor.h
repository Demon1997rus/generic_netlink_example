#pragma once

#include <QObject>

#include "../lib/clientinterface.h"

class Processor : public QObject
{
    Q_OBJECT
public:
    explicit Processor(QObject* parent = nullptr);

private:
    QScopedPointer<AbstractInterface> interface;
};
