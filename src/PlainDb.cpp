#include "PlainDb.h"


#include <QList>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>

#include "ibase.h"

static PlainDb *PlainDbInstance = nullptr;

PlainDb::PlainDb()
{

}

PlainDb::~PlainDb()
{
    QString name = db->connectionName();

    while (db->isOpen())
        db->close();

    delete db;
    QSqlDatabase::removeDatabase(name);

    while (fb_shutdown(0, fb_shutrsn_exit_called) == 0)
    {}
}

PlainDb::PlainDb(QSqlDatabase *db_) : db(db_)
{

}

bool PlainDb::CreateDB(QString db_name)
{
    QString filePath = db_name;
    QString userName = "SYSDBA";
    QString password = "MASTERKEY";

    QString databasePath_ = db_name;

    QString queryString;
    queryString += "CREATE DATABASE";
    queryString += " \'" + filePath + "\'";
    queryString += " USER \'" + userName + "\'";
    queryString += " PASSWORD \'" + password + "\'";
    //queryString += " DEFAULT CHARACTER SET UNICODE_FSS";

    ISC_STATUS_ARRAY status;
    isc_db_handle   databaseHandle = NULL;
    isc_tr_handle   transactionHandle = NULL;

    unsigned short g_nFbDialect = SQL_DIALECT_V6;

    if (isc_dsql_execute_immediate(status, &databaseHandle, &transactionHandle, 0, queryString.toStdString().c_str (), g_nFbDialect, NULL))
    {
        long SQLCODE = isc_sqlcode(status);
        return false;
    }

    isc_commit_transaction( status, &transactionHandle );
    return true;
}

PlainDb * PlainDb::getInstance()
{
    if (PlainDbInstance == nullptr)
    {
        //qDebug() << QSqlDatabase::drivers ();
        QString db_name = qApp->applicationDirPath() + QDir::separator()+ "DATA.FDB";
        qDebug() << db_name;
        if (!QFile::exists(db_name))
        {
            if (!CreateDB(db_name))
            {
                qWarning() << QString("Can't create DB");
                qApp->quit();
            }
        }

        QSqlDatabase * datab = new QSqlDatabase();
        *datab = QSqlDatabase::addDatabase("QIBASE");
        //datab->setHostName("localhost");
        datab->setDatabaseName(db_name);
        datab->setUserName("SYSDBA");
        datab->setPassword("MASTERKEY");


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
                    "    END)  AS FULL_NAME, "
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
            + SeachString2 + orderString;
    return temp;

}

void PlainDb::addContact(Contact * con)
{
    QString prepQuery = QString("INSERT INTO \"contact\" (\"TIP\", \"TIMESTAMP\", \"TEL\", \"FAX\", \"ADRES\", \"EMAIL\", \"HTTP\", \"UPLEVEL\", \"NOTES\") VALUES (:tip, :timestamp, :tel, :fax, :adres, :email, :http, :uplevel, :notes) returning \"ID\"");
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
    if(!query.exec())
    {
        QSqlError err = query.lastError();
        QString estr = err.text();
        qDebug() << ("addContact query error ") << estr;
    }
    query.first();
    int id = query.value(0).toInt();
    con->setId(id);

    if (con->getTip() == 0 )
    {
        addFirm(con);
    }
    else
    {
        addMan(con);
    }
    updateImage(con);
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
    if(!query.exec())
    {
        QSqlError err = query.lastError();
        QString estr = err.text();
        qDebug() << ("updateContact query error") << estr;
    }
    if (con->getTip()==0)
    {
        updateFirm(con);
    }
    else
    {
        updateMan(con);
    }
    updateImage(con);
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
    if(!query.exec())
    {
        QSqlError err = query.lastError();
        QString estr = err.text();
        qDebug() << ("getId query error") << estr;
    }

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
    if(!query.exec())
    {
        QSqlError err = query.lastError();
        QString estr = err.text();
        qDebug() << ("getContById query error") << estr;
    }
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
    getImage(tCont);
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
    if(!query.exec())
    {
        QSqlError err = query.lastError();
        QString estr = err.text();
        qDebug() << ("addFirm query error") << estr;
    }
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
    if(!query.exec())
    {
        QSqlError err = query.lastError();
        QString estr = err.text();
        qDebug() << ("addMan query error ") << estr;
    }
}

void PlainDb::addImage(Contact* con)
{
    QString prepQuery = QString("INSERT INTO \"IMAGE\" (\"ID\", \"IMAGE\") VALUES (:id, :image)");
    QSqlQuery query;
    query.prepare(prepQuery);
    query.bindValue(":id",con->getId());
    query.bindValue(":image",con->getIcon());
    if(!query.exec())
    {
        QSqlError err = query.lastError();
        QString estr = err.text();
        qDebug() << ("addMan query error ") << estr;
    }
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
    QString prepQuery = QString("SELECT * FROM \"man\" WHERE \"id\" = :id");
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

void PlainDb::getImage(Contact& con)
{
    QString prepQuery = QString("SELECT \"IMAGE\" FROM \"IMAGE\" WHERE \"ID\" = :id");
    QSqlQuery query;
    query.prepare(prepQuery);
    query.bindValue(":id",con.getId());
    query.exec();
    query.first();
    if (query.isNull(0))
    {
        //con.setIcon(QByteArray());
        return;
    }
    QByteArray outByteArray = query.value( 0 ).toByteArray();
    //QPixmap outPixmap = QPixmap();
    //outPixmap.loadFromData( outByteArray );
    con.setIcon(outByteArray);
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
    if(!query.exec())
    {
        QSqlError err = query.lastError();
        QString estr = err.text();
        qDebug() << ("updateFirm query error ") << estr;
    }

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
    if(!query.exec())
    {
        QSqlError err = query.lastError();
        QString estr = err.text();
        qDebug() << ("updateMan query error ") << estr;
    }
}

void PlainDb::updateImage(Contact* con)
{
    //сначало проверим есть аватар уже
    QString prepQuery = QString("SELECT COUNT (*) FROM \"IMAGE\" WHERE \"ID\"=:id ");
    QSqlQuery query;
    query.prepare(prepQuery);
    query.bindValue(":id", con->getId());
    if( !query.exec() )
    {
        QSqlError err = query.lastError();
        QString estr = err.text();
        qDebug() << ("updateImage query SELECT error ") << estr;
    }
    query.first();
    //принимаем решение обновлять или добавлять
//    int tt =query.value(0).toInt();
    if ( query.value(0).toInt() <= 0 )//result()->isValid())
        addImage(con);
    else
    {
        QString prepQuery = QString("UPDATE \"IMAGE\" SET \"IMAGE\"=:image WHERE \"ID\"=:id ");
        QSqlQuery query;
        query.prepare(prepQuery);
        query.bindValue(":id",con->getId());
        query.bindValue(":image",con->getIcon());
        if(!query.exec())
        {
            QSqlError err = query.lastError();
            QString estr = err.text();
            qDebug() << ("updateImage query UPDATE error ") << estr;
        }
    }
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
    if(!query.exec())
    {
        QSqlError err = query.lastError();
        QString estr = err.text();
        qDebug() << ("changeContactTip query error ") << estr;
    }

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
    if(!query.exec())
    {
        QSqlError err = query.lastError();
        QString estr = err.text();
        qDebug() << ("deleteFirm query error ") << estr;
    }
}

void PlainDb::deleteMan(Contact *con)
{
    QString prepQuery = QString("DELETE FROM \"man\" WHERE \"id\"=:id ");
    QSqlQuery query;
    query.prepare(prepQuery);
    query.bindValue(":id",con->getId());
    if(!query.exec())
    {
        QSqlError err = query.lastError();
        QString estr = err.text();
        qDebug() << ("deleteMan query error ") << estr;
    }
}

void PlainDb::deleteImage(Contact *con)
{
    QString prepQuery = QString("DELETE FROM \"IMAGE\" WHERE \"ID\"=:id ");
    QSqlQuery query;
    query.prepare(prepQuery);
    query.bindValue(":id",con->getId());
    if(!query.exec())
    {
        QSqlError err = query.lastError();
        QString estr = err.text();
        qDebug() << ("deleteMan query error ") << estr;
    }
}


void PlainDb::deleteContact(Contact * con)
{
    QString prepQuery = QString("DELETE FROM \"contact\" WHERE \"ID\"=:id ");
    QSqlQuery query;
    query.prepare(prepQuery);
    query.bindValue(":id",con->getId());
    if(!query.exec())
    {
        QSqlError err = query.lastError();
        QString estr = err.text();
        qDebug() << ("deleteContact query error ") << estr;
    }

    if (con->getTip()==0)
    {
        deleteFirm(con);
    }
    else
    {
        deleteMan(con);
    }
    deleteImage(con);
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
                    "LEFT JOIN \"firm\" ON \"contact\".\"ID\" = \"firm\".\"id\" "
                    "LEFT JOIN \"man\" ON \"contact\".\"ID\" = \"man\".\"id\" " //);
                    "WHERE \"contact\".\"ID\" <> :id");

    QSqlQuery query;
    query.prepare(prep);
    query.bindValue(":id",selfId);
    if(!query.exec())
    {
        QSqlError err = query.lastError();
        QString estr = err.text();
        qDebug() << ("GetAcsList query error ") << estr;
    }


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
    if(!query.exec())
    {
        QSqlError err = query.lastError();
        QString estr = err.text();
        qDebug() << ("GetContactsListByUplevel query error ") << estr;
    }

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
    if(!query.exec())
    {
        QSqlError err = query.lastError();
        QString estr = err.text();
        qDebug() << ("HasChildByUplevel query error ") << estr;
    }
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

void PlainDb::SetOrderString(int col)
{
    QString order = "ASC";
    if (orderString.contains(order))
    {
        order = "DESC";
    }
    orderString.clear();
    switch (col) {
    case 2:
        orderString = " ORDER BY FULL_NAME " + order;
        break;
    case 3:
        orderString = " ORDER BY \"contact\".\"TEL\" " + order;
        break;
    case 4:
        orderString = " ORDER BY \"contact\".\"FAX\" " + order;
        break;
    case 5:
        orderString = " ORDER BY \"contact\".\"EMAIL\" " + order;
        break;
    case 6:
        orderString = " ORDER BY \"contact\".\"HTTP\" " + order;
        break;
    case 7:
        orderString = " ORDER BY \"contact\".\"NOTES\" " + order;
        break;
    default:
        break;
    }
}
