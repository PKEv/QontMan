#include "vcard.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QDebug>

VCard::VCard()
{

}

VCard::~VCard()
{

}

void VCard::Export(const QString fileName, Contact * con)
{
    QFile output(fileName);
    if (output.open(QFile::WriteOnly | QFile::Text))
    {
        QTextStream out(&output);
        out << VC_BEGIN_TOKEN << endl;
        out << VC_VERSION << endl;
        out << VC_FORMATTED_NAME << VC_KEY_END << con->getFullName() << endl;
        out << VC_NAME << VC_KEY_END << con->getName1()
            << VC_SEPARATOR_TOKEN << con->getName2()
            << VC_SEPARATOR_TOKEN << con->getName3() << endl;
        out << VC_ADDRESS << VC_KEY_END << con->getAdr().split(";",QString::SkipEmptyParts).join(",") << endl;
        out << VC_EMAIL << VC_KEY_END << con->getEmail().split(";",QString::SkipEmptyParts).join(",") << endl;
        out << VC_NOTE << VC_KEY_END << con->getZametka().trimmed() << endl;
        out << VC_TELEPHONE << VC_KEY_END << con->getTel().split(";",QString::SkipEmptyParts).join(",") << endl;
        out << VC_TELEPHONE << VC_KEY_END << con->getFax().split(";",QString::SkipEmptyParts).join(",") << endl;
        out << VC_URL << VC_KEY_END << con->getHttp().split(";",QString::SkipEmptyParts).join(",") << endl;
        out << VC_END_TOKEN << endl;

        output.close();
        QMessageBox::information(0, QObject::tr("Экспорт произведён"), QObject::tr("Контакт был экспортирован."));
    }
}

void VCard::Import(const QString fileName, Contact * con)
{
    QFile input(fileName);
    if (!input.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::information(0, QObject::tr("Ошибка"), QObject::tr("Невозможно открыть файл"));
        return;
    }
    QTextStream in(&input);
    QString line = in.readLine();
    if (!QString::compare(line, VC_BEGIN_TOKEN, Qt::CaseInsensitive) == 0)
    {
        QMessageBox::information(0, QObject::tr("Ошибка"), QObject::tr("Неверная структура файла"));
        return;
    }
    line = in.readLine();
    if (!QString::compare(line,VC_VERSION, Qt::CaseInsensitive) == 0)
    {
        QMessageBox::information(0, QObject::tr("Ошибка"), QObject::tr("Неверная структура (версия) файла"));
        return;
    }
    line = in.readLine();
    while(!in.atEnd() && QString::compare(line,VC_END_TOKEN, Qt::CaseInsensitive) != 0)
    {
        QStringList fields = line.split(":");
        if (QString(fields.at(0)).compare(VC_FORMATTED_NAME, Qt::CaseInsensitive) == 0 )
        {
            con->setFullName(fields.at(1));
        }
        else if (QString(fields.at(0)).split(";").at(0).compare(VC_NAME, Qt::CaseInsensitive) == 0 )
        {
            QStringList nameFields = QString(fields.at(1)).split(";");
            if (nameFields.size() >= 1)
                con->setName1(nameFields.at(0));
            if (nameFields.size() >= 2)
                con->setName2(nameFields.at(1));
            if (nameFields.size() >= 3)
                con->setName3(nameFields.at(2));
        }
        else if (QString(fields.at(0)).split(";").at(0).compare(VC_ADDRESS, Qt::CaseInsensitive) == 0 )
        {
            QStringList fieldsTemp = con->getAdr().split(";", QString::SkipEmptyParts);
            fieldsTemp.push_back(fields.at(1));
            con->setAdr(fieldsTemp.join(";"));
        }
        else if (QString(fields.at(0)).split(";").at(0).compare(VC_EMAIL, Qt::CaseInsensitive) == 0 )
        {
            QStringList fieldsTemp = con->getEmail().split(";", QString::SkipEmptyParts);
            fieldsTemp.push_back(fields.at(1));
            con->setEmail(fieldsTemp.join(";"));
        }
        else if (QString(fields.at(0)).split(";").at(0).compare(VC_NOTE, Qt::CaseInsensitive) == 0 )
        {
            con->setZametka(fields.at(1));
        }
        else if (QString(fields.at(0)).split(";").at(0).compare(VC_TELEPHONE, Qt::CaseInsensitive) == 0 )
        {
            QStringList fieldsTemp = con->getTel().split(";", QString::SkipEmptyParts);
            fieldsTemp.push_back(fields.at(1));
            con->setTel(fieldsTemp.join(";"));
        }
        else if (QString(fields.at(0)).split(";").at(0).compare(VC_URL, Qt::CaseInsensitive) == 0 )
        {
            fields.removeAt(0);
            QString http = fields.join(":");
            QStringList fieldsTemp = con->getHttp().split(";", QString::SkipEmptyParts);
            fieldsTemp.push_back(http);
            con->setHttp(fieldsTemp.join(";"));
        }
        else if (QString(fields.at(0)).split(";").at(0).compare(VC_PHOTO, Qt::CaseInsensitive) == 0 )
        {
            QString data = QString(fields.at(1));
            QByteArray temp = QByteArray(data.toLocal8Bit().constData());
            con->setIcon(temp);
        }

        line = in.readLine();
    }


    input.close();
}
