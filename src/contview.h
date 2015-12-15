#ifndef CONTVIEW_H
#define CONTVIEW_H

#include <QDialog>
#include "contact.h"
#include <qabstractbutton.h>

namespace Ui {
class ContView;
}

class ContView : public QDialog
{
    Q_OBJECT

public:
    explicit ContView(QWidget *parent = 0);
    ContView(Contact * tcont);
    ~ContView();
    Contact* getContact();

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::ContView *ui;
    void Fill();
    Contact * cont;
};

#endif // CONTVIEW_H
