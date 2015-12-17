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
                    "        WHEN 0 THEN "
                    "            firm.shortname "
                    "        WHEN 1 THEN "
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
    QString prepQuery = QString("INSERT INTO contact (tip, timestamp, tel, fax, adres, email, http, uplevel, notes) VALUES (:tip, :timestamp, :tel, :fax, :adres, :email, :http, :uplevel, :notes)");
    QSqlQuery query;
    query.prepare(prepQuery);
    query.bindValue(":tip",con->getTip());
    query.bindValue(":timestamp",con->getDate());
    query.bindValue(":tel",con->getTel());
    query.bindValue(":fax",con->getFax());
    query.bindValue(":adres",con->getAdr());
    query.bindValue(":email",con->getEmail());
    query.bindValue(":http",con->getHttp());
    query.bindValue(":uplevel",con->getUpLevel());
    query.bindValue(":notes",con->getZametka());
    query.exec();
    int id = query.lastInsertId().toInt();
    con->setId(id);

    if (con->getTip() == 0 )
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
    QString prepQuery = QString("SELECT id FROM contact WHERE tip=:tip AND timestamp=:timestamp AND tel=:tel AND fax=:fax AND adres=:adres AND email=:email AND http=:http AND uplevel=:uplevel AND notes=:notes");
    QSqlQuery query;
    query.clear();
    query.prepare(prepQuery);
    query.bindValue(":tip",con->getTip(),QSql::Out);
    query.bindValue(":timestamp",con->getDate());
    query.bindValue(":tel",con->getTel());
    query.bindValue(":fax",con->getFax());
    query.bindValue(":adres",con->getAdr());
    query.bindValue(":email",con->getEmail());
    query.bindValue(":http",con->getHttp());
    query.bindValue(":uplevel",con->getUpLevel());
    query.bindValue(":notes",con->getZametka());
    query.exec();

    query.first();
    return query.value(0).toInt();
}

void PlainDb::addFirm(Contact* con)
{
    QString prepQuery = QString("INSERT INTO firm (id, sobstv, name, shortname) VALUES (:id, :sobstv, :name, :shortname)");
    QSqlQuery query;
    query.prepare(prepQuery);
    query.bindValue(":id",con->getId());
    query.bindValue(":sobstv",con->getName1());
    query.bindValue(":name",con->getName2());
    query.bindValue(":shortname",con->getFullName());
    query.exec();
}

void PlainDb::addMan(Contact* con)
{
    QString prepQuery = QString("INSERT INTO man (id, name, surname, patronymic, shortname) VALUES (:id, :name, :surname, :patronymic, :shortname)");
    QSqlQuery query;
    query.prepare(prepQuery);
    query.bindValue(":id",con->getId());
    query.bindValue(":name",con->getName1());
    query.bindValue(":surname",con->getName2());
    query.bindValue(":patronymic",con->getName3());
    query.bindValue(":shortname",con->getFullName());
    query.exec();
}
