#include <Phonon>

#include "collection.h"
#include "player.h"
#include "playlistItem.h"

Player::Player()
    :m_playlist()
    ,m_currentItem(0)
    ,m_collection()
{
    m_mediaObject = Phonon::createPlayer(Phonon::MusicCategory);
    connect(m_mediaObject, SIGNAL(aboutToFinish()), this, SLOT(enqueueNext()));
}

Player::~Player()
{
    delete m_mediaObject;
}

bool Player::add(QString url)
{
    /**
     * Purpose:
     *  Add a single file from the database to the playlist. This command increments the playlist version by 1 for each song added to the playlist.
     */
    if (!m_collection.urlIsValid(url))
        return false;

    PlaylistItem newFile(url, ++m_playlistVersion);
    m_playlist.append(newFile);

    return true;
}

int Player::addId(QString url, int pos = -1)
{
    /**
     * Purpose:
     *  Like add, but returns an id.
     */
    if (!m_collection.urlIsValid(url))
        return false;
    if ((pos < 0) || (pos > m_playlist.size())){
        if (m_currentItem + 1 < m_playlist.size())
            pos = m_currentItem + 1;
        else
            pos = m_playlist.size();
    }

    PlaylistItem newItem(url, ++m_playlistVersion);
    return m_playlistVersion;
}

void Player::clear()
{
    /**
     * Purpose:
     *  Clears the current playlist, increment the playlist version by 1.
     */
    m_playlist.clear();
    m_mediaObject->clearQueue();
    ++m_playlistVersion;
}

QString Player::currentSong()
{
    /**
     * Purpose:
     *  Displays the metadata of the current song.
     *
     * Example return value:
     *  >currentsong
     *  file: albums/bob_marley/songs_of_freedom/disc_four/12.bob_marley_-_could_you_be_loved_(12"_mix).flac
     *  Time: 327
     *  Album: Songs Of Freedom - Disc Four
     *  Artist: Bob Marley
     *  Title: Could You Be Loved (12" Mix)
     *  Track: 12
     *  Pos: 11
     *  Id: 6601
     *  OK
     */

    return m_collection.find("filename", m_playlist[m_currentItem]);
}
            
bool Player::removeItem(int num)
{
    /**
     * Purpose:
     *  Removes item number "num" from the playlist.
     */

    if (num < 0 || num > m_playlist.size())
        return false;

    m_playlist.removeAt(num);
    ++m_playlistVersion;
    return true;

}

bool Player::removeId(int id)
{
    for (int i=0; i<m_playlist.size(); ++i)
        if (m_playlist[i].getId() == id){
            m_playlist.removeAt(i);
            return true;
        }
    return true;
}

bool Player::loadPlaylist(QString path)
{
    //TODO: implement
    return false;
}

bool Player::renamePlaylist(QString oldPath, QString newPath)
{
    //TODO: implement
    return false;
}

bool Player::move(int oldPos, int newPos)
{
    if (oldPos < 0 || oldPos > m_playlist.size() || newPos < 0 || newPos > m_playlist.size())
        return false;
    m_playlist.move(oldPos, newPos);
    return true;
}

bool Player::moveId(int id, int newPos)
{
    for (int i=0; i<m_playlist.size(); ++i)
        if (m_playlist[i].getId() == id)
        {
            m_playlist.move(i, newPos);
            return true;
        }
    return false;
}

QString Player::playlistInfo(int song = -1)
{
    if (song < 0 || song > m_playlist.size())
    {
        QString result;
        for (int i=0; i<m_playlist.size(); ++i)
            result.append(m_collection.find("filename", m_playlist[i]));
        return result;
    }
    else
        return m_collection.find("filename", m_playlist[song]);
}

void Player::enqueueNext() //slot
{
    /**
     * Purpose:
     *  This is responsible for enqueuing the next song, before the previous one stops.
     */

    if (++m_currentItem > m_playlist.size())
        m_currentItem = 0;
    m_mediaObject->enqueue(m_playlist[m_currentItem]);
}
