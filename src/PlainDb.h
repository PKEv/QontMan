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
    //static void resetInstance();
    PlainDb(QSqlDatabase *db);
    QString getQuery();

    void addContact(Contact * con);
    void updateContact(Contact * con);
    void changeContactTip(Contact * con);
    void deleteContact(Contact * con);
    Contact getContById(const int id);

    void setSeachString(QString str);

    void GetContactsListByUplevel(std::vector<Contact> &vec, const int uplevel);
    bool HasChildByUplevel(const int uplevel);

    void SetFilterByListId(QStringList & list);

    void GetAcsList(std::vector<QString> &vec, const int selfId);

    QSqlDatabase *db;
private:

    int getId(Contact* con);

    void addFirm(Contact* con);
    void addMan(Contact* con);

    void getFirm(Contact &con);
    void getMan(Contact &con);

    void updateFirm(Contact *con);
    void updateMan(Contact* con);

    void deleteFirm(Contact *con);
    void deleteMan(Contact *con);
    QString SeachString;
    QString SeachString2;
};

#endif // PlainDb_H
