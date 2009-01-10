#include <QtSql>

#include "collection.h"

Collection::Collection(QObject *parent)
    : QObject(parent)
{
    m_db = QSqlDatabase::addDatabase("QPSQL");
    m_db.setHostName("cirkus");
    m_db.setDatabaseName("locutus");
    m_db.setUserName("sandsmark_impd");
    m_db.setPassword("Kohdie3o");
    if (!m_db.open())
        emit databaseError(m_db.lastError().text());

    m_validTypes << "album"
                 << "artist"
                 << "title"
                 << "filename";
}

Collection::~Collection()
{
    m_db.close();
}


QString Collection::find(QString type, QString term) {
    /**
     * Purpose:
     *  Finds songs in the database with a case sensitive, exact match to "term".
     *
     * Example return value:
     *  >find artist "Bob Marley"
     *  file: albums/bob_marley/songs_of_freedom/disc_two/18.bob_marley_-_i_shot_the_sheriff_(live).flac
     *  Time: 326
     *  Album: Songs Of Freedom - Disc Two
     *  Artist: Bob Marley
     *  Title: I Shot The Sheriff (Live)
     *  Track: 18
     */
    if (!m_validTypes.contains(type))
        return QString();

    QString result;

    QSqlQuery query(m_db);
    query.prepare("SELECT filename, duration, album, artist, title, tracknumber FROM file WHERE ?=?");
    query.addBindValue(type);
    query.addBindValue(term);

    query.exec();
    while (query.next()) 
    {
        result.append(QString("file: %1\nTime: %2\nAlbum: %3\nArtist: %4\nTitle: %5\nTrack: %6\n").arg(
                query.value(0).toString(),              // File
                QString(query.value(1).toInt()/1000),   // Time
                query.value(2).toString(),              // Album
                query.value(3).toString(),              // Artist
                query.value(4).toString(),              // Title
                query.value(5).toString()));             // Track
    }
    return result;
}

QString Collection::list(QString metadataType, QString termType = "", QString term = "")
{
    /**
     * Purpose :
     *  List all metadata of type "metadataType" (eg. album), optionally matching "termType" (eg. artist) "term".
     *
     * Example return value:
     *  >list album artist Tool
     *  Album: Lateralus
     *  Album: ænema
     */

    if ((!m_validTypes.contains(metadataType)) || (!m_validTypes.contains(termType)))
        return QString();

    QString type = metadataType; 
    type[0] = type[0].toUpper(); // eg.: album → Album
    // alternative implementation: type[0] &= ~32; ;D

    QString result;

    QSqlQuery query(m_db);
    query.prepare("SELECT DISTINCT ? FROM file WHERE '?'='?'");
    query.addBindValue(metadataType);
    query.addBindValue(termType);
    query.addBindValue(term);

    query.exec();
    while (query.next())
    {
        result.append(QString("%1: %2\n").arg(type, query.value(0).toString()));
    }

    return result;
}

QString Collection::listAll(QString path)
{
    /**
     * Purpose:
     *  Lists all directories and filenames in "path" recursively.
     *
     * Example return value:
     * >listall
     * [lots of output depending on how many files are in database]
     * file: vocal/onion_radio_news/the_onion_radio_news_-_bastards_pick_wrong_man.mp3
     */

    QString result;

    QSqlQuery query(m_db);
    query.prepare("SELECT filename FROM file WHERE filename LIKE '^?'"); //FIXME?
    query.addBindValue(path);

    query.exec();
    while (query.next())
    {
        result.append(QString("file: %1").arg(query.value(0).toString()));
    }
    return result;
}

QString Collection::listAllInfo(QString path)
{
    /**
     * Purpose:
     *  Lists all information in database about all music files in "path" recursively.
     *
     * Example return value:
     *  >listallinfo
     *  [Lots of output depending on how many files are in the database]
     *  file: vocal/onion_radio_news/the_onion_radio_news_-_bastards_pick_wrong_man.mp3
     *  Time: 36
     *  Artist: Onion Radio News
     *  Title: Bastards Pick Wrong Man to Mess With
     *  Genre: Comedy
     */

    QString result;

    QSqlQuery query(m_db);
    query.prepare("SELECT filename, duration, album, artist, title, tracknumber FROM file WHERE filename LIKE '^?'");//FIXME?
    query.addBindValue(path);

    query.exec();
    while (query.next()) 
    {
        result.append(QString("file: %1\nTime: %2\nAlbum: %3\nArtist: %4\nTitle: %5\nTrack: %6\n").arg(
                query.value(0).toString(),              // File
                QString(query.value(1).toInt()/1000),   // Time
                query.value(2).toString(),              // Album
                query.value(3).toString(),              // Artist
                query.value(4).toString(),              // Title
                query.value(5).toString()));             // Track
    }
    return result;
}

QString Collection::lsInfo(QString path = "/")
{
    /**
     * Purpose:
     *  List contents of <string directory>, from the database. 
     *
     * Example return value:
     *  >lsinfo
     *  directory: albums
     *  directory: individuals
     *  directory: live
     *  directory: vocal
    */
    QString result;

    QSqlQuery query(m_db);
//    query.prepare("SELECT DISTINCT SUBSTRING(filename FROM ?([^/]*)/) FROM file WHERE filename LIKE ?%");//FIXME? (fnutteproblemet)
    query.prepare("SELECT DISTINCT substring(filename from '^?([^/]*)'), substring(filename from '^?([^/]*)/') is null from file where filename like '?%'");
    query.addBindValue(path);
    query.addBindValue(path);
    query.addBindValue(path);

    query.exec();
    while(query.next())
    {
        if (query.value(1).toBool())
            result.append(QString("file: %1").arg(query.value(0).toString()));
        else
            result.append(QString("directory: %1").arg(query.value(0).toString()));
    }
    return result;
}

QString Collection::search(QString type, QString term)
{
    /**
     * Purpose:
     *  Finds songs in the database with a case insensitive match to "term".
     *
     * Example return value:
     *  >search filename bastards_pick_wrong_man
     *  file: vocal/onion_radio_news/the_onion_radio_news_-_bastards_pick_wrong_man.mp3
     *  Time: 36
     *  Artist: Onion Radio News 
     *  Title: Bastards Pick Wrong Man to Mess With
     *  Genre: Comedy
     */

    if (!m_validTypes.contains(type) && type != "any")
        return QString();

    QString result;

    if (type == "any")
    {
        type = "(";
        for (int i=0; i<m_validTypes.size()-1 ; ++i)
            type.append(QString(" %1 ||").arg(m_validTypes.at(i)));
        type.append(QString(" %1 )").arg(m_validTypes.last()));
    }

    QSqlQuery query(m_db);
    query.prepare("SELECT filename, duration, album, artist, title, tracknumber FROM file WHERE ? ILIKE ?");
    query.addBindValue(type);
    query.addBindValue(term);

    query.exec();
    while (query.next()) 
    {
        result.append(QString("file: %1\nTime: %2\nAlbum: %3\nArtist: %4\nTitle: %5\nTrack: %6\n").arg(
                query.value(0).toString(),              // File
                QString(query.value(1).toInt()/1000),   // Time
                query.value(2).toString(),              // Album
                query.value(3).toString(),              // Artist
                query.value(4).toString(),              // Title
                query.value(5).toString()));             // Track
    }
    return result;
}
