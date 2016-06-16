#include "vcard.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <iostream>
#include <QDebug>
#include <QRegularExpression>

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
    in.setCodec("UTF-8");
    in.setAutoDetectUnicode(true);
    QString line = in.readLine();
    if (!QString::compare(line, VC_BEGIN_TOKEN, Qt::CaseInsensitive) == 0)
    {
        QMessageBox::information(0, QObject::tr("Ошибка"), QObject::tr("Неверная структура файла"));
        return;
    }
    line = in.readLine();
    if (!QString::compare(line.split(":").at(0),VC_VER, Qt::CaseInsensitive) == 0)
    {
        QMessageBox::information(0, QObject::tr("Ошибка"), QObject::tr("Неверная структура (версия) файла"));
        return;
    }
    //line = in.readLine();
    ReadLine(in,line);
    while(!in.atEnd() && QString::compare(line,VC_END_TOKEN, Qt::CaseInsensitive) != 0)
    {
        QStringList fields = line.split(":");
        if (QString(fields.at(0)).split(";").at(0).compare(VC_FORMATTED_NAME, Qt::CaseInsensitive) == 0 )
        {
            if (!QString(fields.at(0)).contains(VC_Quoted_Printable, Qt::CaseInsensitive))
                con->setFullName(fields.at(1));
            else
                con->setFullName(DecodeQuotedPrintable(fields.at(1)));
        }
        else if (QString(fields.at(0)).split(";").at(0).compare(VC_NAME, Qt::CaseInsensitive) == 0 )
        {
            QStringList nameFields = QString(fields.at(1)).split(";");
            if (!line.contains(VC_Quoted_Printable, Qt::CaseInsensitive))
            {
                if (nameFields.size() >= 1)
                    con->setName1(nameFields.at(0));
                if (nameFields.size() >= 2)
                    con->setName2(nameFields.at(1));
                if (nameFields.size() >= 3)
                    con->setName3(nameFields.at(2));
            }
            else
            {
                if (nameFields.size() >= 1)
                    con->setName1(DecodeQuotedPrintable(nameFields.at(0)));
                if (nameFields.size() >= 2)
                    con->setName2(DecodeQuotedPrintable(nameFields.at(1)));
                if (nameFields.size() >= 3)
                    con->setName3(DecodeQuotedPrintable(nameFields.at(2)));
            }
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
            QByteArray temp;
            temp.append((QByteArray(data.toLocal8Bit().constData())));
            data = in.readLine();
            //data = data.trimmed();
            while(!data.isEmpty())
            {
                temp.append((QByteArray(data.toLocal8Bit().constData())));
                data = in.readLine();
                //data = data.trimmed();
            }
            QByteArray temp2 = QByteArray::fromBase64(temp);
            con->setIcon(temp2);
        }

        ReadLine(in,line);
        //line = in.readLine();
    }


    input.close();
}

QString VCard::DecodeQuotedPrintable(const QString &input1)
{
    QString input = RemoveLineWraping(input1);
    static int hexVal[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0, 0, 0, 0, 0, 0, 10, 11, 12, 13, 14, 15};
    QByteArray output;
    int i;

    for ( i = 0; i < input.length(); ++i)
    {
        if (QChar(input.at(i).toLatin1()) == '=')
        {
            int bit1 = ++i;
            int bit2 = ++i;
            output.append((hexVal[input.at(bit1).toLatin1() - '0'] << 4) + hexVal[input.at(bit2).toLatin1() - '0']);
        }
        else
        {
            output.append(input.at(i).toLatin1());
        }
    }
    return output.toStdString().c_str();
}
QString VCard::RemoveLineWraping(const QString &line)
{
    QString out = line;
    return out.replace(QRegularExpression("=="),"=");
}

void VCard::ReadLine(QTextStream &in, QString &line)
{
    line = in.readLine();
    auto pos = in.pos();
    QString temp1 = in.readLine().trimmed();
    while (!ContainKey(temp1) && !in.atEnd())
    {
        pos = in.pos();
        line += temp1.trimmed();
        temp1 = in.readLine();

    }
    in.seek(pos);
}
bool VCard::ContainKey(const QString &line)
{
    if (line.isEmpty())
        return false;
    if (line.contains(QRegularExpression("^"+QString(VC_ADDRESS)+"(:|;)")))
        return true;
    if (line.contains(QRegularExpression("^"+QString(VC_END_TOKEN))))
        return true;
    if (line.contains(QRegularExpression("^"+QString(VC_AGENT)+"(:|;)")))
        return true;
    if (line.contains(QRegularExpression("^"+QString(VC_BIRTHDAY)+"(:|;)")))
        return true;
    if (line.contains(QRegularExpression("^"+QString(VC_CATEGORIES)+"(:|;)")))
        return true;
    if (line.contains(QRegularExpression("^"+QString(VC_CLASS)+"(:|;)")))
        return true;
    if (line.contains(QRegularExpression("^"+QString(VC_DELIVERY_LABEL)+"(:|;)")))
        return true;
    if (line.contains(QRegularExpression("^"+QString(VC_EMAIL)+"(:|;)")))
        return true;
    if (line.contains(QRegularExpression("^"+QString(VC_FORMATTED_NAME)+"(:|;)")))
        return true;
    if (line.contains(QRegularExpression("^"+QString(VC_GEOGRAPHIC_POSITION)+"(:|;)")))
        return true;
    if (line.contains(QRegularExpression("^"+QString(VC_KEY)+"(:|;)")))
        return true;
    if (line.contains(QRegularExpression("^"+QString(VC_LOGO)+"(:|;)")))
        return true;
    if (line.contains(QRegularExpression("^"+QString(VC_MAILER)+"(:|;)")))
        return true;
    if (line.contains(QRegularExpression("^"+QString(VC_NAME)+"(:|;)*")))
        return true;
    if (line.contains(QRegularExpression("^"+QString(VC_NICKNAME)+"(:|;)")))
        return true;
    if (line.contains(QRegularExpression("^"+QString(VC_NOTE)+"(:|;)")))
        return true;
    if (line.contains(QRegularExpression("^"+QString(VC_ORGANIZATION)+"(:|;)")))
        return true;
    if (line.contains(QRegularExpression("^"+QString(VC_PHOTO)+"(:|;)")))
        return true;
    if (line.contains(QRegularExpression("^"+QString(VC_PRODUCT_IDENTIFIER)+"(:|;)")))
        return true;
    if (line.contains(QRegularExpression("^"+QString(VC_REVISION)+"(:|;)")))
        return true;
    if (line.contains(QRegularExpression("^"+QString(VC_ROLE)+"(:|;)")))
        return true;
    if (line.contains(QRegularExpression("^"+QString(VC_SORT_STRING)+"(:|;)")))
        return true;
    if (line.contains(QRegularExpression("^"+QString(VC_SOUND)+"(:|;)")))
        return true;
    if (line.contains(QRegularExpression("^"+QString(VC_TELEPHONE)+"(:|;)")))
        return true;
    if (line.contains(QRegularExpression("^"+QString(VC_TIME_ZONE)+"(:|;)")))
        return true;
    if (line.contains(QRegularExpression("^"+QString(VC_TITLE)+"(:|;)")))
        return true;
    if (line.contains(QRegularExpression("^"+QString(VC_URL)+"(:|;)")))
        return true;
    if (line.contains(QRegularExpression("^"+QString(VC_CUSTOM)+"*:*")))
        return true;

    return false;
}
