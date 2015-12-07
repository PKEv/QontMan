#include "PlainDb.h"


#include <QList>
#include <QSqlError>

static PlainDb *PlainDbInstance = nullptr;

PlainDb::PlainDb()
{

}

PlainDb::~PlainDb()
{

}

PlainDb::PlainDb(QSqlDatabase *db_) : db(db_)
{

}

PlainDb * PlainDb::getInstance()
{
    if (PlainDbInstance == nullptr)
    {
        QList<QString> initLst;
        initLst.push_back(QString("CREATE TABLE IF NOT EXISTS contact (id INTEGER PRIMARY KEY AUTOINCREMENT, tip INTEGER, timestamp INTEGER NOT NULL, ") +
                          QString("tel  TEXT, fax TEXT, adres TEXT, email TEXT, http TEXT, uplevel TEXT, notes TEXT);"));
        initLst.push_back(QString("CREATE TABLE IF NOT EXISTS firm (id INTEGER NOT NULL, sobstv TEXT, name TEXT, shortname TEXT);"));
        initLst.push_back(QString("CREATE TABLE IF NOT EXISTS man (id INTEGER NOT NULL, name TEXT, surname TEXT, patronymic TEXT, shortname TEXT);"));
        initLst.push_back(QString("CREATE TABLE IF NOT EXISTS avatar (id INTEGER NOT NULL, img BLOB);"));

        QSqlDatabase *datab = new QSqlDatabase();
        *datab = QSqlDatabase::addDatabase("QSQLITE");
        datab->setConnectOptions("QSQLITE_ENABLE_SHARED_CACHE=1");

        QString db_name = qApp->applicationDirPath() + "//test.db";
        datab->setDatabaseName(db_name);

        if (!datab->open())
        {
            qWarning() << QString("Can't open file: %1").arg(db_name);
            datab->setDatabaseName(":memory:");
            datab->setConnectOptions();
            datab->open();
        }

        for (const QString &cmd : initLst)
        {
            datab->exec(cmd);
        }
        PlainDbInstance = new PlainDb(datab);
    }
    return PlainDbInstance;
}

QString PlainDb::getQuery()
{
    return QString ("SELECT contact.id, contact.tip, "
                    "    (CASE contact.tip "
                    "        WHEN 1 THEN "
                    "            firm.shortname "
                    "        WHEN 0 THEN "
                    "            man.shortname "
                    "        ELSE 'bad type' "
                    "    END), "
                    "    contact.tel, contact.fax, contact.email, contact.http, contact.notes "
                    "FROM contact "
                    "LEFT JOIN firm ON contact.id = firm.id "
                    "LEFT JOIN man ON contact.id = man.id ");
}
