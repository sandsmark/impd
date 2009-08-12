#ifndef PLAYER_H
#define PLAYER_H

#include <Phonon>

#include "playlistItem.h"
#include "collection.h"

class Player : Phonon::MediaObject
{
    Q_OBJECT

    public:
        Player();
        ~Player();

        bool add(QString url);
        int addId(QString url, int pos);
        void clear();
        QString currentSong();
        bool removeItem(int num);
        bool removeId(int id);
        bool loadPlaylist(QString url);
        bool renamePlaylist(QString oldName, QString newName);
        bool move(int oldPos, int newPos);
        bool moveId(int songId, int newPos);
        QString playlistInfo(int song);
        void play(int id=-1);

    private slots:
        void enqueueNext();
    private:
        Phonon::MediaObject *m_mediaObject;
        QList<PlaylistItem> m_playlist;
        int m_currentItem;
        Collection m_collection;
        int m_playlistVersion;

};

#endif
