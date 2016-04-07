#ifndef PASSPORT_H
#define PASSPORT_H

#include <QDialog>
#include <QLabel>
#include <QIcon>

#include "contact.h"

namespace Ui {
class passport;
}

class passport : public QDialog
{
    Q_OBJECT

public:
    explicit passport(QWidget *parent = 0);
    passport(Contact * tcont);
    ~passport();

private:
    void fill();
    Ui::passport *ui;
    Contact * cont;
    QLabel *shotNameLabel = nullptr;
    QLabel *telTitle = nullptr;
    QLabel *telValue = nullptr;
    QLabel *faxTitle = nullptr;
    QLabel *faxValue = nullptr;
    QLabel *httpTitle = nullptr;
    QLabel *httpValue = nullptr;
    QLabel *emlTitle = nullptr;
    QLabel *emlValue = nullptr;
    QLabel *adrTitle = nullptr;
    QLabel *adrValue = nullptr;
    QLabel *zamTitle = nullptr;
    QLabel *zamValue = nullptr;
    QLabel   *icon = nullptr;

};

#endif // PASSPORT_H
