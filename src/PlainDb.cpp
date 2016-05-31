#include "PlainDb.h"


#include <QList>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>

#include "settings.h"

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
        qDebug() << QSqlDatabase::drivers ();
        QString db_name = qApp->applicationDirPath() + "//TEST.FDB";

        QSqlDatabase *datab = new QSqlDatabase();
         *datab = QSqlDatabase::addDatabase("QIBASE");
        datab->setHostName("localhost");
        datab->setDatabaseName(db_name);
        datab->setUserName("SYSDBA");
        datab->setPassword("masterkey");


        if (!datab->open())
        {
            qWarning() << QString("Can't open file: %1");
            qDebug() << datab->lastError().text();
            return 0;
        }

        PlainDbInstance = new PlainDb(datab);
    }
    return PlainDbInstance;
}

QString PlainDb::getQuery()
{
    QString temp =  QString ("SELECT \"contact\".\"ID\", \"contact\".\"TIP\", "
                    "    (CASE \"contact\".\"TIP\" "
                    "        WHEN 0 THEN "
                    "            \"firm\".\"shortname\" "
                    "        WHEN 1 THEN "
                    "            \"man\".\"shortname\" "
                    "        ELSE null "
                    "    END), "
                    "    \"contact\".\"TEL\", \"contact\".\"FAX\", \"contact\".\"EMAIL\", \"contact\".\"HTTP\", \"contact\".\"NOTES\" "
                    "FROM \"contact\" "
                    "LEFT JOIN \"firm\" ON \"contact\".\"ID\" = \"firm\".\"id\" "
                    "LEFT JOIN \"man\" ON \"contact\".\"ID\" = \"man\".\"id\" "
                    "WHERE (     UPPER (\"contact\".\"TEL\")     LIKE '%"+SeachString+"%' "
                             "OR UPPER (\"contact\".\"FAX\")     LIKE '%"+SeachString+"%' "
                             "OR UPPER (\"contact\".\"EMAIL\")   LIKE '%"+SeachString+"%' "
                             "OR UPPER (\"contact\".\"HTTP\")    LIKE '%"+SeachString+"%' "
                             "OR UPPER (\"contact\".\"NOTES\")   LIKE '%"+SeachString+"%' "
                             "OR UPPER (\"firm\".\"sobstv\")     LIKE '%"+SeachString+"%' "
                             "OR UPPER (\"firm\".\"name\")       LIKE '%"+SeachString+"%' "
                             "OR UPPER (\"firm\".\"shortname\")  LIKE '%"+SeachString+"%' "
                             "OR UPPER (\"man\".\"name\")        LIKE '%"+SeachString+"%' "
                             "OR UPPER (\"man\".\"surname\")     LIKE '%"+SeachString+"%' "
                             "OR UPPER (\"man\".\"patronymic\")  LIKE '%"+SeachString+"%' "
                             "OR UPPER (\"man\".\"shortname\")   LIKE '%"+SeachString+"%' ) ")
            + SeachString2;
    return temp;

}

void PlainDb::addContact(Contact * con)
{
    QString prepQuery = QString("INSERT INTO \"contact\" (\"TIP\", \"TIMESTAMP\", \"TEL\", \"FAX\", \"ADRES\", \"EMAIL\", \"HTTP\", \"UPLEVEL\", \"NOTES\") VALUES (:tip, :timestamp, :tel, :fax, :adres, :email, :http, :uplevel, :notes)");
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
    QString prepQuery = QString("UPDATE \"contact\" SET \"TIP\"=:tip, \"TIMESTAMP\"=:timestamp, \"TEL\"=:tel, \"FAX\"=:fax, \"ADRES\"=:adres, \"EMAIL\"=:email, \"HTTP\"=:http, \"UPLEVEL\"=:uplevel, \"NOTES\"=:notes WHERE \"ID\" =:id");
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
    QString prepQuery = QString("SELECT \"ID\" FROM \"contact\" WHERE \"TIP\"=:tip AND \"TIMESTAMP\"=:timestamp AND \"TEL\"=:tel AND \"FAX\"=:fax AND \"ADRES\"=:adres AND \"EMAIL\"=:email AND \"HTTP\"=:http AND \"UPLEVEL\"=:uplevel AND \"NOTES\"=:notes");
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
    QString prepQuery = QString ("SELECT \"TIP\", \"ADRES\", \"TIMESTAMP\", \"TEL\", \"FAX\", \"EMAIL\", \"HTTP\", \"NOTES\", \"UPLEVEL\" "
                                 "FROM \"contact\" WHERE \"contact\".\"ID\"=:id ");
    QSqlQuery query;
    query.prepare(prepQuery);
    query.bindValue(":id",id);
    query.exec();
    query.first();

    Contact tCont;
    tCont.setId(id);
    tCont.setTip(query.record().value("TIP").toInt());
    tCont.setTel(query.record().value("TEL").toString());
    tCont.setAdr(query.record().value("ADRES").toString());
    tCont.setFax(query.record().value("FAX").toString());
    tCont.setEmail(query.record().value("EMAIL").toString());
    tCont.setHttp(query.record().value("HTTP").toString());
    tCont.setZametka(query.record().value("NOTES").toString());
    tCont.setDate(query.record().value("TIMESTAMP").toString());
    tCont.setUpLevel(query.record().value("UPLEVEL").toInt());
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
    QString prepQuery = QString("INSERT INTO \"firm\" (\"id\", \"sobstv\", \"name\", \"shortname\") VALUES (:id, :sobstv, :name, :shortname)");
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
    QString prepQuery = QString("INSERT INTO \"man\" (\"id\", \"name\", \"surname\", \"patronymic\", \"shortname\") VALUES (:id, :name, :surname, :patronymic, :shortname)");
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
    QString prepQuery = QString("SELECT * FROM \"firm\" WHERE \"id\" = :id");
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
    QString prepQuery = QString("SELECT \"name\", \"surname\", \"patronymic\", \"shortname\"  FROM \"man\" WHERE \"id\" = :id");
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
    QString prepQuery = QString("UPDATE \"firm\" SET \"sobstv\"=:sobstv, \"name\"=:name, \"shortname\"=:shortname WHERE \"id\"=:id");
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
    QString prepQuery = QString("UPDATE \"man\" SET \"name\"=:name, \"surname\"=:surname, \"patronymic\"=:patronymic, \"shortname\"=:shortname WHERE \"id\"=:id ");
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
    QString prepQuery = QString("UPDATE \"contact\" SET \"TIP\"=:tip, \"TIMESTAMP\"=:timestamp, \"TEL\"=:tel, \"FAX\"=:fax, \"ADRES\"=:adres, \"EMAIL\"=:email, \"HTTP\"=:http, \"UPLEVEL\"=:uplevel, \"NOTES\"=:notes WHERE \"ID\"=:id");
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
    QString prepQuery = QString("DELETE FROM \"firm\" WHERE \"id\"=:id ");
    QSqlQuery query;
    query.prepare(prepQuery);
    query.bindValue(":id",con->getId());
    query.exec();
}

void PlainDb::deleteMan(Contact *con)
{
    QString prepQuery = QString("DELETE FROM \"man\" WHERE \"id\"=:id ");
    QSqlQuery query;
    query.prepare(prepQuery);
    query.bindValue(":id",con->getId());
    query.exec();
}

void PlainDb::deleteContact(Contact * con)
{
    QString prepQuery = QString("DELETE FROM \"contact\" WHERE \"ID\"=:id ");
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
    SeachString = str.toUpper();
}

void PlainDb::GetAcsList(std::vector<QString> &vec, const int selfId)
{
    vec.clear();
    QString prep = QString("SELECT \"contact\".\"ID\", \"contact\".\"TIP\", "
                    "    (CASE \"contact\".\"TIP\" "
                    "        WHEN 0 THEN "
                    "            \"firm\".\"shortname\" "
                    "        WHEN 1 THEN "
                    "            \"man\".\"shortname\" "
                    "        ELSE null "
                    "    END) "
                    "FROM \"contact\" "
                    "LEFT JOIN firm ON \"contact\".\"ID\" = \"firm\".\"id\" "
                    "LEFT JOIN man ON \"contact\".\"ID\" = \"man\".\"id\" " //);
                    "WHERE \"contact\".\"ID\" <> :id");

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
    QString prep = QString("SELECT \"contact\".\"ID\" "
                    "FROM \"contact\" "
                    "WHERE \"contact\".\"UPLEVEL\" = :uplevel");

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
    QString prep = QString("SELECT \"contact\".\"ID\" "
                    "FROM \"contact\" "
                    "WHERE \"contact\".\"UPLEVEL\" = :uplevel");

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
        SeachString2 += QString("\"contact\".\"ID\"=%1").arg(entry);
    }
    SeachString2 += ")";
}
