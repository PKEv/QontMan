#include "contview.h"
#include "ui_contview.h"

ContView::ContView(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ContView)
{
    ui->setupUi(this);
}

ContView::ContView(Contact * tcont)
{
    cont = tcont;
}


ContView::~ContView()
{
    delete ui;
}

void ContView::Fill()
{
    ui->lineEdit->setText(cont->getFullName());
    ui->comboBox->addItem(cont->getTel());
    ui->comboBox_2->addItem(cont->getFax());
    ui->comboBox_3->addItem(cont->getAdr());
    ui->comboBox_4->addItem(cont->getEmail());
    ui->comboBox_5->addItem(cont->getHttp());

}

Contact *ContView::getContact()
{
    return cont;
}

void ContView::on_buttonBox_clicked(QAbstractButton *button)
{
    if (ui->buttonBox->standardButton(button) == QDialogButtonBox::Ok)
    {
        cont->setFullName(ui->lineEdit->text());
        cont->setTel(ui->comboBox->itemText(0));
        cont->setFax(ui->comboBox_2->itemText(0));
        cont->setAdr(ui->comboBox_3->itemText(0));
    }

}
