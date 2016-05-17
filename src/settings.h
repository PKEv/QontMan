#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>


class Settings
{
public:
    Settings();
    ~Settings();
    QString getDbPuth();
    //void write();

private:
    QSettings *sett;
};

#endif // SETTINGS_H
