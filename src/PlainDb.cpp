#include "PlainDb.h"


#include <QList>
#include <QSqlError>
#include <QSqlQuery>

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

void PlainDb::addContact(Contact * con)
{
    QString prepQuery = QString("INSERT INTO contact (tip, timestamp, tel, fax, adres, email, http, uplevel, notes) VALUES (%1, %2, %3, '%4', '%5', '%6', '%7', '%8', '%9');")
            .arg(con->getTip())
            .arg(con->getDate())
            .arg(con->getTel())
            .arg(con->getFax())
            .arg(con->getEmail())
            .arg(con->getHttp())
            .arg(con->getUpLevel())
            .arg(con->getZametka());
    QSqlQuery query;
    query.exec(prepQuery);

    con->setId(getId(con));

    if (con->getId() == 0 )
    {
        addFirm(con);
    }
    else
    {
        addMan(con);
    }
}

int PlainDb::getId(Contact* con)
{
    QString prepQuery = QString("SELECT id IN contact WHERE tip='%1' AND timestamp='%2' AND tel='%3' AND fax='%4' AND adres='%5' AND email='%6' AND http='%7' AND uplevel='%8' AND notes='%9';")
            .arg(con->getTip())
            .arg(con->getDate())
            .arg(con->getTel())
            .arg(con->getFax())
            .arg(con->getEmail())
            .arg(con->getHttp())
            .arg(con->getUpLevel())
            .arg(con->getZametka());
    QSqlQuery query;
    query.exec(prepQuery);
    if (query.size() != 1)
        return -1;
    query.first();
    return query.value(0).toInt();
}

void PlainDb::addFirm(Contact* con)
{
    QString prepQuery = QString("INSERT INTO firm (id, sobstv, name, shortname) VALUES (%1, %2, %3, '%4');")
            .arg(con->getId())
            .arg(con->getName1())
            .arg(con->getName2())
            .arg(con->getFullName());

    QSqlQuery query;
    query.exec(prepQuery);
}

void PlainDb::addMan(Contact* con)
{
    QString prepQuery = QString("INSERT INTO man (id, name, surname, patronymic, shortname) VALUES (%1, %2, %3, '%4', '%5');")
            .arg(con->getId())
            .arg(con->getName1())
            .arg(con->getName2())
            .arg(con->getName3())
            .arg(con->getFullName());
    QSqlQuery query;
    query.exec(prepQuery);
}
