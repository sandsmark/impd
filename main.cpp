#include <QApplication>
#include <QtCore>
#include <phonon>

#include <stdlib.h>

#include "dialog.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Dialog dialog;
    dialog.show();
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    return dialog.exec();
}
