#ifndef PASSPORT_H
#define PASSPORT_H

#include <QDialog>
#include <QLabel>

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
    QLabel *shotNameLabel;
    QLabel *telTitle;
    QLabel *telValue;
    QLabel *faxTitle;
    QLabel *faxValue;
    QLabel *httpTitle;
    QLabel *httpValue;
    QLabel *emlTitle;
    QLabel *emlValue;
    QLabel *adrTitle;
    QLabel *adrValue;
    QLabel *zamTitle;
    QLabel *zamValue;

};

#endif // PASSPORT_H
