#ifndef CONTVIEW_H
#define CONTVIEW_H

#include <QDialog>
#include <QComboBox>
#include <QSignalMapper>
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
    void plusButton(QWidget *comboBox);
    void minusButton(QWidget *comboBox);

private:
    Ui::ContView *ui;
    void Fill();
    Contact * cont;
    QSignalMapper *signalMapper;
    QSignalMapper *signalMapper2;
};

#endif // CONTVIEW_H
