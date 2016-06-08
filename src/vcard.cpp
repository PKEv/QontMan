#include "vcard.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

VCard::VCard()
{

}

VCard::VCard(Contact & con) : VCard()
{
    this->con = &con;
}
VCard::~VCard()
{
    if (con)
        delete con;
}

void VCard::Export(const QString fileName)
{
    QFile output(fileName);
    if (output.open(QFile::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&output);
        out << VC_BEGIN_TOKEN << endl;
        out << VC_VERSION << endl;
        out << VC_FORMATTED_NAME << con->getFullName() << endl;
        out << VC_NAME << con->getName1()
            << VC_SEPARATOR_TOKEN << con->getName2()
            << VC_SEPARATOR_TOKEN << con->getName3() << endl;
        out << VC_ADDRESS << con->getAdr().split(";",QString::SkipEmptyParts).join(",") << endl;
        out << VC_EMAIL << con->getEmail().split(";",QString::SkipEmptyParts).join(",") << endl;
        out << VC_NOTE << con->getZametka().trimmed() << endl;
        out << VC_TELEPHONE << con->getTel().split(";",QString::SkipEmptyParts).join(",") << endl;
        out << VC_TELEPHONE << con->getFax().split(";",QString::SkipEmptyParts).join(",") << endl;
        out << VC_URL << con->getHttp().split(";",QString::SkipEmptyParts).join(",") << endl;
        out << VC_END_TOKEN << endl;

        output.close();
    }
}

void VCard::Import(const QString fileName, Contact * con)
{

}
