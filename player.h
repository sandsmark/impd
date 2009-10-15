#ifndef PLAYER_H
#define PLAYER_H

#include <Phonon>

#include "playlistItem.h"
#include "collection.h"

class Player : public QObject
{
    Q_OBJECT

    public:
        Player();
        ~Player();

        void add(QString url);
        int addId(QString url, int pos);
        void clear();
        QString currentSong();
        void removeItem(int num);
        void removeId(int id);
        void loadPlaylist(QString url);
        void renamePlaylist(QString oldName, QString newName);
        void move(int oldPos, int newPos);
        QString playlistInfo(int song);
        void play(int id=-1);
        void pause();
        void stop();
        void togglePause();
        void next();
        void previous();
        void seek(int amount);
        void seek(int amount, int song);
        void seekId(int amount, int id);

        void moveId(int songId, int newPos);

        void setRandom(bool value);
        void setConsume(bool value);
        void setRepeat(bool value);
        QString status();


    private slots:
        void enqueueNext();

    private:
        Phonon::MediaObject *m_mediaObject;
        QList<QString> m_playlist;
        int m_currentItem;
        Collection m_collection;
        int m_playlistVersion;

        bool m_random;
        bool m_consume;
        bool m_repeat;

};

#endif
