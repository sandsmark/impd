#include "server.h"
#include "clientthread.h"

#include <stdlib.h>

Server::Server(QObject *parent)
    : QTcpServer(parent)
{
    m_player = new Player();
}

Server::~Server()
{
    delete m_player;
}

void Server::incomingConnection(int socketDescriptor)
{
    ClientThread *thread = new ClientThread(socketDescriptor, this, m_player, m_collection);

    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    connect(thread, SIGNAL(stop()), this, SLOT(lol()));

    thread->start();
}

void Server::lol(int file)
{
    qDebug() << "lol";
}
