#include "clientthread.h"

#include <QtNetwork>
#include <iostream>

ClientThread::ClientThread(int socketDescriptor, QObject *parent)
    : QThread(parent), socketDescriptor(socketDescriptor), m_running(false)
{
}

void ClientThread::run()
{
    QTcpSocket tcpSocket;
    if (!tcpSocket.setSocketDescriptor(socketDescriptor)) {
        emit error(tcpSocket.error());
        return;
    }

    // Send greetings
    tcpSocket.write("OK MPD 0.12.2\n");

    QString command, response;

    while (m_running && (tcpSocket.state() == QAbstractSocket::ConnectedState)) {
        m_running = tcpSocket.waitForReadyRead();   // Wait for command, 
                                                    // if none is received until timeout
                                                    // (default 30 seconds, stop running).

        command = QString(tcpSocket.readLine()).trimmed();
       
        qDebug() << command;

        response = "OK\n"; //Default response, nothing went wrong.

        if (command.startsWith("play")) {
            int num = -1;
            if (command.length() > 4)
                num = command.mid(4).toInt(); 
            qDebug() << "Playing number: " << num;
            emit play(num);
        }
        else if (command.startsWith("pause")) {
            if (command.length() > 6 && (command.at(6) == '1'))
                emit pause();
            else if (command.length() > 6 && (command.at(6) == '0'))
                emit play(-1);
            else emit togglePause();
        }
        else if (command.startsWith("stop"))
            emit stop();
    }

    tcpSocket.disconnectFromHost();
}

