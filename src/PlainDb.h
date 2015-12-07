#ifndef PlainDb_H
#define PlainDb_H

#include <QDebug>
#include <QtSql/QtSql>
#include <QString>
#include <QString>

class PlainDb
{
public:
    PlainDb();
    ~PlainDb();
    static PlainDb* getInstance();
    static void resetInstance();
    PlainDb(QSqlDatabase *db);
    static QString getQuery();

    QSqlDatabase *db;

};

#endif // PlainDb_H
