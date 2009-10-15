#ifndef SERVER_H
#define SERVER_H

#include "player.h"
#include "collection.h"

#include <QStringList>
#include <QTcpServer>
#include <Phonon>

class Server : public QTcpServer
{
    Q_OBJECT

public:
    Server(QObject *parent = 0);
    ~Server();

public slots:
    void lol(int);

protected:
    void incomingConnection(int socketDescriptor);

private:
    Player *m_player;
    Collection *m_collection;
};

#endif
