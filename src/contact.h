#ifndef CONTACT_H
#define CONTACT_H

#include <QString>
#include <QDateTime>

class Contact
{
public:
    Contact();
    ~Contact();

    void setId(int tid);
    int getId();

    void setDate(QString tdate);
    QString getDate();

    void setFullName(QString tname);
    QString getFullName();
    QString autoFullName();

    void setTip(int ttip);
    int getTip();

    void setTel(QString ttel);
    QString getTel();

    void setFax(QString tfax);
    QString getFax();

    void setAdr(QString tadr);
    QString getAdr();

    void setEmail(QString temail);
    QString getEmail();

    void setHttp(QString thttp);
    QString getHttp();

    void setUpLevel(int tuplevel);
    int getUpLevel();

    void setZametka(QString tzametka);
    QString getZametka();

    void setName1(QString tName1);
    QString getName1();

    void setName2(QString tName2);
    QString getName2();

    void setName3(QString tName3);
    QString getName3();


private:
    int id;
    QString date;
    QString fullName;
    int tip;
    QString tel;
    QString fax;
    QString adres;
    QString email;
    QString http;
    int upLevel;
    QString zametka;

    QString name1;
    QString name2;
    QString name3;
};

#endif // CONTACT_H
