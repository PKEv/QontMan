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
    QString temp =  QString ("SELECT contact.id, contact.tip, "
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
                    "LEFT JOIN man ON contact.id = man.id "
                    "WHERE (contact.tel         LIKE '%"+SeachString+"%' "
                             "OR contact.fax     LIKE '%"+SeachString+"%' "
                             "OR contact.email   LIKE '%"+SeachString+"%' "
                             "OR contact.http    LIKE '%"+SeachString+"%' "
                             "OR contact.notes   LIKE '%"+SeachString+"%' "
                             "OR firm.sobstv     LIKE '%"+SeachString+"%' "
                             "OR firm.name       LIKE '%"+SeachString+"%' "
                             "OR firm.shortname  LIKE '%"+SeachString+"%' "
                             "OR man.name        LIKE '%"+SeachString+"%' "
                             "OR man.surname     LIKE '%"+SeachString+"%' "
                             "OR man.patronymic  LIKE '%"+SeachString+"%' "
                             "OR man.shortname   LIKE '%"+SeachString+"%' ) ")
            + SeachString2;

    return temp;
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

void PlainDb::updateContact(Contact * con)
{
    QString prepQuery = QString("UPDATE contact SET tip=:tip, timestamp=:timestamp, tel=:tel, fax=:fax, adres=:adres, email=:email, http=:http, uplevel=:uplevel, notes=:notes WHERE id =:id");
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
    query.bindValue(":id",con->getId());
    query.exec();
    if (con->getTip()==0)
    {
        updateFirm(con);
    }
    else
    {
        updateMan(con);
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

Contact PlainDb::getContById(const int id)
{
    QString prepQuery = QString ("SELECT tip, adres, timestamp, tel, fax, email, http, notes, uplevel "
                                 "FROM contact WHERE contact.id=:id ");
    QSqlQuery query;
    query.prepare(prepQuery);
    query.bindValue(":id",id);
    query.exec();
    query.first();

    Contact tCont;
    tCont.setId(id);
    tCont.setTip(query.record().value("tip").toInt());
    tCont.setTel(query.record().value("tel").toString());
    tCont.setAdr(query.record().value("adres").toString());
    tCont.setFax(query.record().value("fax").toString());
    tCont.setEmail(query.record().value("email").toString());
    tCont.setHttp(query.record().value("http").toString());
    tCont.setZametka(query.record().value("notes").toString());
    tCont.setDate(query.record().value("timestamp").toString());
    tCont.setUpLevel(query.record().value("uplevel").toInt());
    if (tCont.getTip()==0)
    {
        getFirm(tCont);
    }
    else
    {
        getMan(tCont);
    }
    return tCont;
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

void PlainDb::getFirm(Contact &con)
{
    QString prepQuery = QString("SELECT * FROM firm WHERE id = :id");
    QSqlQuery query;
    query.prepare(prepQuery);
    query.bindValue(":id",con.getId());
    query.exec();
    query.first();
    con.setName1(query.record().value("sobstv").toString());
    con.setName2(query.record().value("name").toString());
    con.setFullName(query.record().value("shortname").toString());
}

void PlainDb::getMan(Contact& con)
{
    QString prepQuery = QString("SELECT name, surname, patronymic, shortname  FROM man WHERE id = :id");
    QSqlQuery query;
    query.prepare(prepQuery);
    query.bindValue(":id",con.getId());
    query.exec();
    query.first();
    con.setName1(query.record().value("name").toString());
    con.setName2(query.record().value("surname").toString());
    con.setName3(query.record().value("patronymic").toString());
    con.setFullName(query.record().value("shortname").toString());
}

void PlainDb::updateFirm(Contact *con)
{
    QString prepQuery = QString("UPDATE firm SET sobstv=:sobstv, name=:name, shortname=:shortname WHERE id=:id");
    QSqlQuery query;
    query.prepare(prepQuery);
    query.bindValue(":id",con->getId());
    query.bindValue(":sobstv",con->getName1());
    query.bindValue(":name",con->getName2());
    query.bindValue(":shortname",con->getFullName());
    query.exec();
}

void PlainDb::updateMan(Contact* con)
{
    QString prepQuery = QString("UPDATE man SET name=:name, surname=:surname, patronymic=:patronymic, shortname=:shortname WHERE id=:id ");
    QSqlQuery query;
    query.prepare(prepQuery);
    query.bindValue(":id",con->getId());
    query.bindValue(":name",con->getName1());
    query.bindValue(":surname",con->getName2());
    query.bindValue(":patronymic",con->getName3());
    query.bindValue(":shortname",con->getFullName());
    query.exec();
}

void PlainDb::changeContactTip(Contact * con)
{
    QString prepQuery = QString("UPDATE contact SET tip=:tip, timestamp=:timestamp, tel=:tel, fax=:fax, adres=:adres, email=:email, http=:http, uplevel=:uplevel, notes=:notes WHERE id =:id");
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
    query.bindValue(":id",con->getId());
    query.exec();
    if (con->getTip()==0)
    {
        deleteMan(con);
        addFirm(con);
    }
    else
    {
        deleteFirm(con);
        addMan(con);
    }

}

void PlainDb::deleteFirm(Contact *con)
{
    QString prepQuery = QString("DELETE FROM firm WHERE id=:id ");
    QSqlQuery query;
    query.prepare(prepQuery);
    query.bindValue(":id",con->getId());
    query.exec();
}

void PlainDb::deleteMan(Contact *con)
{
    QString prepQuery = QString("DELETE FROM man WHERE id=:id ");
    QSqlQuery query;
    query.prepare(prepQuery);
    query.bindValue(":id",con->getId());
    query.exec();
}

void PlainDb::deleteContact(Contact * con)
{
    QString prepQuery = QString("DELETE FROM contact WHERE id=:id ");
    QSqlQuery query;
    query.prepare(prepQuery);
    query.bindValue(":id",con->getId());
    query.exec();
    if (con->getTip()==0)
    {
        deleteFirm(con);
    }
    else
    {
        deleteMan(con);
    }
}

void PlainDb::setSeachString(QString str)
{
    SeachString = str;
}

void PlainDb::GetAcsList(std::vector<QString> &vec, const int selfId)
{
    vec.clear();
    QString prep = QString("SELECT contact.id, contact.tip, "
                    "    (CASE contact.tip "
                    "        WHEN 0 THEN "
                    "            firm.shortname "
                    "        WHEN 1 THEN "
                    "            man.shortname "
                    "        ELSE 'bad type' "
                    "    END) "
                    "FROM contact "
                    "LEFT JOIN firm ON contact.id = firm.id "
                    "LEFT JOIN man ON contact.id = man.id " //);
                    "WHERE contact.id <> :id");

    QSqlQuery query;
    query.prepare(prep);
    query.bindValue(":id",selfId);
    query.exec();

    while(query.next())
    {
        int id = query.record().value(0).toInt();
        QString name = query.record().value(2).toString();
        vec.push_back(QString("%1:" + name).arg(id));
    }
}

void PlainDb::GetContactsListByUplevel(std::vector<Contact> &vec, const int uplevel)
{
    vec.clear();
    QString prep = QString("SELECT contact.id "
                    "FROM contact "
                    "WHERE contact.uplevel = :uplevel");

    QSqlQuery query;
    query.prepare(prep);
    query.bindValue(":uplevel",uplevel);
    query.exec();

    while(query.next())
    {
        int id = query.record().value(0).toInt();
        Contact temp = getContById(id);
        vec.push_back(temp);
    }

}

bool PlainDb::HasChildByUplevel(const int uplevel)
{
    QString prep = QString("SELECT contact.id "
                    "FROM contact "
                    "WHERE contact.uplevel = :uplevel");

    QSqlQuery query;
    query.prepare(prep);
    query.bindValue(":uplevel", uplevel);
    query.exec();
    return query.first();
}

void PlainDb::SetFilterByListId(QStringList & list)
{
    SeachString2.clear();
    if (list.isEmpty())
        return;
    SeachString2 = " AND (";
    for (const QString& entry: list)
    {
        if (entry != list.first())
            SeachString2 += " OR ";
        SeachString2 += QString("contact.id=%1").arg(entry);
    }
    SeachString2 += ")";
}
