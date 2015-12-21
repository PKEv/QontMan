#ifndef PlainDb_H
#define PlainDb_H

#include <QDebug>
#include <QtSql/QtSql>
#include <QString>
#include <QString>
#include "contact.h"

class PlainDb
{
public:
    PlainDb();
    ~PlainDb();
    static PlainDb* getInstance();
    static void resetInstance();
    PlainDb(QSqlDatabase *db);
    static QString getQuery();
    void addContact(Contact * con);
    void updateContact(Contact * con);
    Contact getContById(const int id);

    QSqlDatabase *db;
private:
    int getId(Contact* con);

    void addFirm(Contact* con);
    void addMan(Contact* con);

    void getFirm(Contact &con);
    void getMan(Contact &con);

    void updateFirm(Contact *con);
    void updateMan(Contact* con);
};

#endif // PlainDb_H
