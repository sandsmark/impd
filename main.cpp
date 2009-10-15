#include <QApplication>
#include <QtCore>
#include <phonon>

#include <stdlib.h>

#include "server.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QCoreApplication::setApplicationName("IMPD");

    Server server;
    if (!server.listen(QHostAddress::Any, 1337)) {
        qFatal("Unable to listen on local port!");
    }
    return app.exec();
}
