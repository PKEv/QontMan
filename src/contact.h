#ifndef CONTACT_H
#define CONTACT_H

#include <QString>
#include <QIcon>
#include <QDateTime>

class Contact
{
public:
    Contact();
    ~Contact();

    void setId(int tid);
    int getId() const;

    void setDate(QString tdate);
    QString getDate();

    void setFullName(QString tname);
    QString getFullName();

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
    int getUpLevel() const;

    void setZametka(QString tzametka);
    QString getZametka();

    void setName1(QString tName1);
    QString getName1();

    void setName2(QString tName2);
    QString getName2();

    void setName3(QString tName3);
    QString getName3();

    void setIcon(QPixmap &pixmap);
    QIcon getIcon();

    bool operator ==(const Contact &another) const;


private:
    int id{0};
    QString date{""};
    QString fullName{""};
    int tip{0};
    QString tel{""};
    QString fax{""};
    QString adres{""};
    QString email{""};
    QString http{""};
    int upLevel{0};
    QString zametka{""};

    QString name1{""};
    QString name2{""};
    QString name3{""};
    QIcon icon;
};

#endif // CONTACT_H
