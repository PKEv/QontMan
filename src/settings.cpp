#include "settings.h"

Settings::Settings()
{
    sett = new QSettings("settings.ini",QSettings::IniFormat);
}

Settings::~Settings()
{
    sett->sync();
    delete sett;
}


QString Settings::getDbPuth()
{
    return sett->value("DbPuth", "test.db").toString();
}
