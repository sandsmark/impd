#ifndef SERVER_H
#define SERVER_H

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
    Phonon::MediaObject *m_player;
};

#endif
