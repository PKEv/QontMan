#ifndef VCARD_H
#define VCARD_H

#include "contact.h"

#define VC_SEPARATOR_TOKEN ';'
#define VC_KEY_END ':'
#define VC_END_LINE_TOKEN '\n'
#define VC_BEGIN_TOKEN "BEGIN:VCARD"
#define VC_VERSION     "VERSION:3.0"
#define VC_END_TOKEN   "END:VCARD"

#define VC_ADDRESS              "ADR"
#define VC_AGENT                "AGENT"
#define VC_BIRTHDAY             "BDAY"
#define VC_CATEGORIES           "CATEGORIES"
#define VC_CLASS                "CLASS"
#define VC_DELIVERY_LABEL       "LABEL"
#define VC_EMAIL                "EMAIL"
#define VC_FORMATTED_NAME       "FN"
#define VC_GEOGRAPHIC_POSITION  "GEO"
#define VC_KEY                  "KEY"
#define VC_LOGO                 "LOGO"
#define VC_MAILER               "MAILER"
#define VC_NAME                 "N"
#define VC_NICKNAME             "NICKNAME"
#define VC_NOTE                 "NOTE"
#define VC_ORGANIZATION         "ORG"
#define VC_PHOTO                "PHOTO"
#define VC_PRODUCT_IDENTIFIER   "PRODID"
#define VC_REVISION             "REV"
#define VC_ROLE                 "ROLE"
#define VC_SORT_STRING          "SORT-STRING"
#define VC_SOUND                "SOUND"
#define VC_TELEPHONE            "TEL"
#define VC_TIME_ZONE            "TZ"
#define VC_TITLE                "TITLE"
#define VC_URL                  "URL"


class VCard
{
public:
    VCard();
    ~VCard();
    void static Export(const QString fileName, Contact * con);
    void static Import(const QString fileName, Contact * con);
private:

};

#endif // VCARD_H
