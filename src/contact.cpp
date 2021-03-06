#include "contact.h"

Contact::Contact()
{

}

Contact::~Contact()
{

}

void Contact::setId(int tid)
{
    id = tid;
}

int Contact::getId() const
{
    return id;
}

void Contact::setDate(QString tdate)
{
    date = tdate;
}

QString Contact::getDate()
{
    return date;
}

void Contact::setFullName(QString tname)
{
    fullName = tname;
}

QString Contact::getFullName()
{
    return fullName;
}

void Contact::setTip(int ttip)
{
    tip = ttip;
}

int Contact::getTip()
{
    return tip;
}

void Contact::setTel(QString ttel)
{
    tel = ttel;
}

QString Contact::getTel()
{
    return tel;
}

void Contact::setFax(QString tfax)
{
    fax = tfax;
}

QString Contact::getFax()
{
    return fax;
}

void Contact::setAdr(QString tadr)
{
    adres = tadr;
}

QString Contact::getAdr()
{
    return adres;
}

void Contact::setEmail(QString temail)
{
    email = temail;
}

QString Contact::getEmail()
{
    return email;
}

void Contact::setHttp(QString thttp)
{
    http = thttp;
}

QString Contact::getHttp()
{
    return http;
}

void Contact::setUpLevel(int tuplevel)
{
    upLevel = tuplevel;
}

int Contact::getUpLevel() const
{
    return upLevel;
}

void Contact::setZametka(QString tzametka)
{
    zametka = tzametka;
}

QString Contact::getZametka()
{
    return zametka;
}

void Contact::setName1(QString tName1)
{
    name1 = tName1;
}

QString Contact::getName1()
{
    return name1;
}

void Contact::setName2(QString tName2)
{
    name2 = tName2;
}

QString Contact::getName2()
{
    return name2;
}

void Contact::setName3(QString tName3)
{
    name3 = tName3;
}

QString Contact::getName3()
{
    return name3;
}

bool Contact::operator ==(const Contact& another) const
{
    if (this->getId() == another.getId() && this->id !=0)
        return true;
    return false;
}

void Contact::setIcon(QByteArray &pixmap)
{
    icon = pixmap;
}

QByteArray Contact::getIcon()
{
    return icon;
}
