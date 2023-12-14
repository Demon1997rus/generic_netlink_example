#include <QCoreApplication>
#include <QDir>
#include <QString>
#include <QDebug>

#include "processor.h"

int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);
    new Processor(&app);
    return app.exec();
}
