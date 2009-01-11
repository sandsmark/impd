#ifndef PLAYLISTITEM_H
#define PLAYLISTITEM_H

#include <QString>

/**
 * Stupid convenience class, because somehow each playlist item needs an id.
 */

class PlaylistItem : public QString
{
    public:
        PlaylistItem(QString url, int id) : QString(url), m_id(id) {};
        
        int getId() { return m_id; }

    private:
        int m_id;
};

#endif
