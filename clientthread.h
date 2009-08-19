/*
 * ITKomiteen's MPD
 * Copyright 2009 ITKomiteen ved Studentersamfundet i Trondheim
 *
 * Written by Martin T. Sandsmark
 *
 * May be redistributed under GPLv2 or later.
 * See the COPYING file shipped with this program.
 */

// This is the thread that handles the connecting clients.

#ifndef CLIENTTHREAD_H
#define CLIENTTHREAD_H

#include <QThread>
#include <QTcpSocket>

#include "player.h"
#include "collection.h"

class ClientThread : public QThread
{
    Q_OBJECT

public:
    ClientThread(int socketDescriptor, QObject *parent, Player*, Collection*);

    void run();

signals:
    void error(QTcpSocket::SocketError socketError);
    void play(int);
    void pause();
    void togglePause();
    void stop();


private:
    QString parseCommand(QString command); 

    int m_socketDescriptor;
    bool m_running;
    QString m_currentError;
    Player *m_player;
    Collection *m_collection; 
    QStringList m_validCommands;
};

#endif
