#ifndef COLLECTION_H
#define COLLECTION_H

#include <QtSql>

class Collection : public QObject
{
    Q_OBJECT

    public:
        Collection();
        ~Collection();

        QString find(QString type, QString term); //Finds songs in the database with a case sensitive, exact match to "term".
        QString list(QString metadataType1, QString artist=""); //List all metadata of metadataType*.
        QString listAll(QString path = "/"); // Lists all directories and filenames in "path" recursively. 
        QString listAllInfo(QString path = "/"); //Lists all information in database about all music files in "path" recursively.
        QString lsInfo(QString path = "/"); // List contents of "path", from the database. 
        QString search(QString metadataType, QString term); // Finds songs in the database with a case insensitive match to "term".
        QString count(QString type, QString term); // Retrieve the number of songs and their total playtime in the database matching "term".
        QString validTypes();

        bool urlIsValid(QString url);

        int getFileId(QString url);
        QString getFile(int id);

    signals:
        void databaseError(QString);


    private:
        QSqlDatabase m_db;
        QStringList m_validTypes;
};

#endif
