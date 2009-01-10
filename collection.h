#include <QtSql>

class Collection : public QObject
{
    Q_OBJECT

    public:
        Collection(QObject*);
        ~Collection();

        QString find(QString type, QString term); //Finds songs in the database with a case sensitive, exact match to "term".
        QString list(QString metadataType1, QString metadataType2, QString term); //List all metadata of metadataType*.
        QString listAll(QString path); // Lists all directories and filenames in "path" recursively. 
        QString listAllInfo(QString path); //Lists all information in database about all music files in "path" recursively.
        QString lsInfo(QString path); // List contents of "path", from the database. 
        QString search(QString metadataType, QString term); // Finds songs in the database with a case insensitive match to "term".
        QString count(QString scope, QString term); // Retrieve the number of songs and their total playtime in the database matching "term".

    signals:
        void databaseError(QString);


    private:
        QSqlDatabase m_db;
        QStringList m_validTypes;
};
