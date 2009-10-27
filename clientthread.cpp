/*
 * ITKomiteen's MPD
 * Copyright 2009 ITKomiteen ved Studentersamfundet i Trondheim
 *
 * Written by Martin T. Sandsmark
 *
 * May be redistributed under GPLv2 or later.
 * See the COPYING file shipped with this program.
 */

#include "clientthread.h"

#include <QtNetwork>
#include <iostream>

ClientThread::ClientThread(int socketDescriptor, QObject *parent, Player *player, Collection *collection)
    :QThread(parent),
    m_socketDescriptor(socketDescriptor),
    m_running(false),
    m_player(player),
    m_collection(collection)
{
    m_validCommands << "play"
                    << "pause"
                    << "stop"
                    << "next"
                    << "previous"
                    << "playid"
                    << "seek"
                    << "seekid"
                    << "consume"
                    << "random"
                    << "repeat"
                    << "setvol"
                    << "currentsong"
                    << "clearerror"
                    << "stats"
                    << "count"
                    << "find"
                    << "list"
                    << "listall"
                    << "listallinfo"
                    << "lsinfo"
                    << "search"
                    << "close"
                    << "kill";
}

void ClientThread::run()
{
    QTcpSocket tcpSocket;
    if (!tcpSocket.setSocketDescriptor(m_socketDescriptor)) {
        qWarning() << ":(((";
        emit error(tcpSocket.error());
        return;
    }

    m_running = true;

    QString command, response;

    // Send greetings
    tcpSocket.write("OK MPD 0.12.2\n");

    while (m_running && (tcpSocket.state() == QAbstractSocket::ConnectedState)) {
        m_running = tcpSocket.waitForReadyRead();   // Wait for command, 
                                                    // if none is received until timeout
                                                    // (default 30 seconds, stop running).

        command = QString(tcpSocket.readLine()).trimmed();
       
        qDebug() << command;

        tcpSocket.write(parseCommand(command).toLocal8Bit());
    }

    tcpSocket.disconnectFromHost();
}

QString ClientThread::parseCommand(QString command)
{
    QStringList arguments = command.split(" ");
    command = arguments.takeFirst();

    if (!m_validCommands.contains(command))
        return QString("ACK {unknown command}\n");

    // Playback control
    if (command == "status")
        return m_player->status();
    else if (command == "play") {
        if (arguments.size() > 0)
            m_player->play(arguments[0].toInt());
        else
            m_player->play(-1);
    } else if (command == "pause") {
        if (arguments.size() > 0 && arguments[0] == "1")
            m_player->pause();
        if (arguments.size() > 0 && arguments[0] == "0")
            m_player->play(-1);
        else m_player->togglePause();
    } else if (command == "stop")
        m_player->stop();
    else if (command == "next")
        m_player->next();
    else if (command == "previous")
        m_player->previous();
    else if (command == "playid" && arguments.size() > 0)
        m_player->play(arguments[0].toInt());
    else if (command == "seek" && arguments.size() > 0)
        m_player->seek(arguments[0].toInt());
    else if (command == "seek" && arguments.size() > 1)
        m_player->seek(arguments[0].toInt(), arguments[1].toInt());
    else if (command == "seekid" && arguments.size() > 1)
        m_player->seekId(arguments[0].toInt(), arguments[1].toInt());

    // Playback options
    else if (command == "consume" && arguments.size() > 0)
        m_player->setConsume(arguments[0].toInt());
    else if (command.startsWith("crossfade")) // TODO
        return QString("OK\n");
    else if (command == "random" && arguments.size() > 0)
        m_player->setRandom(arguments[0].toInt());
    else if (command == "repeat" && arguments.size() > 0)
        m_player->setRepeat(arguments[0].toInt());
    else if (command == "setvol" && arguments.size() > 0)
        m_player->setRepeat(arguments[0].toInt());

    // Status
    else if (command.startsWith("currentsong"))
       return m_player->currentSong(); 
    else if (command.startsWith("clearerror"))
        m_currentError = QString();
    else if (command.startsWith("stats")) { //TODO
        QString response;
        response += "artists: -1\n";
        response += "songs: -1\n";
        response += "uptime: -1\n";
        response += "db_playtime: -1\n";
        response += "db_update: -1\n";
        response += "playtime: -1\n";

        return response;
    }

    // Database interaction
    else if (command == "count" && arguments.size() > 1)
        return m_collection->count(arguments[0], arguments[1]);
    else if (command == "find" && arguments.size() > 1)
        return m_collection->find(arguments[0], arguments[1]);
    else if (command == "list") {
        if (arguments.size() > 1)
            return m_collection->list(arguments[0], arguments[1]);
        else if (arguments.size() > 0)
            return m_collection->list(arguments[0]);
    } else if (command == "listall") {
        if (arguments.size() > 0)
            return m_collection->listAll(arguments[0]);
        else 
            return m_collection->listAll();
    } else if (command == "listallinfo") {
        if (arguments.size() > 0)
            return m_collection->listAllInfo(arguments[0]);
        else 
            return m_collection->listAllInfo();
    } else if (command == "lsinfo") {
        if (arguments.size() > 0)
            return m_collection->lsInfo(arguments[0]);
        else 
            return m_collection->lsInfo();
    } else if (command == "search" && arguments.size() > 1)
        return m_collection->find(arguments[0], arguments[1]);

    // Connection settings
    else if (command == "close")
        m_running = false;
    else if (command == "kill")
        qWarning() << "I'm not suicidal (got kill signal from client)";

    // Reflection
    else if (command == "commands")
        return m_validCommands.join("\n") + QString("OK\n");


    // Yeah yeah, everything's under control
    return "OK\n";
}

