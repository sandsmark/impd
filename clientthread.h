#ifndef CLIENTTHREAD_H
#define CLIENTTHREAD_H

#include <QThread>
#include <QTcpSocket>

class ClientThread : public QThread
{
    Q_OBJECT

public:
    ClientThread(int socketDescriptor, QObject *parent);

    void run();

signals:
    void error(QTcpSocket::SocketError socketError);
    void play(int);
    void pause();
    void togglePause();
    void stop();


private:
    int socketDescriptor;
    QString text;
    bool m_running;
};

#endif
