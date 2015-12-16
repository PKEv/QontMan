
#include "contview.h"
#include "ui_contview.h"

ContView::ContView(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ContView)
{
    ui->setupUi(this);
    cont = new Contact;


    signalMapper = new QSignalMapper(this);
    signalMapper2 = new QSignalMapper(this);
    signalMapper->setMapping(ui->plusButton_1, ui->comboBox);
    signalMapper->setMapping(ui->plusButton_2, ui->comboBox_2);
    signalMapper->setMapping(ui->plusButton_3, ui->comboBox_3);
    signalMapper->setMapping(ui->plusButton_4, ui->comboBox_4);
    signalMapper->setMapping(ui->plusButton_5, ui->comboBox_5);
    signalMapper2->setMapping(ui->minusButton_1, ui->comboBox);
    signalMapper2->setMapping(ui->minusButton_2, ui->comboBox_2);
    signalMapper2->setMapping(ui->minusButton_3, ui->comboBox_3);
    signalMapper2->setMapping(ui->minusButton_4, ui->comboBox_4);
    signalMapper2->setMapping(ui->minusButton_5, ui->comboBox_5);
    connect(ui->plusButton_1, SIGNAL(clicked()), signalMapper, SLOT (map()));
    connect(ui->plusButton_2, SIGNAL(clicked()), signalMapper, SLOT (map()));
    connect(ui->plusButton_3, SIGNAL(clicked()), signalMapper, SLOT (map()));
    connect(ui->plusButton_4, SIGNAL(clicked()), signalMapper, SLOT (map()));
    connect(ui->plusButton_5, SIGNAL(clicked()), signalMapper, SLOT (map()));
    connect(ui->minusButton_1, SIGNAL(clicked()), signalMapper2, SLOT (map()));
    connect(ui->minusButton_2, SIGNAL(clicked()), signalMapper2, SLOT (map()));
    connect(ui->minusButton_3, SIGNAL(clicked()), signalMapper2, SLOT (map()));
    connect(ui->minusButton_4, SIGNAL(clicked()), signalMapper2, SLOT (map()));
    connect(ui->minusButton_5, SIGNAL(clicked()), signalMapper2, SLOT (map()));

    connect(signalMapper, SIGNAL(mapped(QWidget*)), this, SLOT(plusButton(QWidget *)));
    connect(signalMapper2, SIGNAL(mapped(QWidget*)), this, SLOT(minusButton(QWidget *)));
}

ContView::ContView(Contact * tcont)
{
    cont = tcont;
}


ContView::~ContView()
{
    delete cont;
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
    {   // заполняем структуру для передачи
        cont->setFullName(ui->lineEdit->text());
        cont->setTel(ui->comboBox->currentText());
        cont->setFax(ui->comboBox_2->currentText());
        cont->setAdr(ui->comboBox_3->currentText());
    }
}

void ContView::plusButton(QWidget *comboBox)
{
    QString temp = qobject_cast<QComboBox*>(comboBox)->currentText();
    if (qobject_cast<QComboBox*>(comboBox)->findText(temp) != -1)
        return;
    qobject_cast<QComboBox*>(comboBox)->clearEditText();
    qobject_cast<QComboBox*>(comboBox)->addItem(temp);
    qobject_cast<QComboBox*>(comboBox)->setCurrentIndex(qobject_cast<QComboBox*>(comboBox)->findText(temp));
    qobject_cast<QComboBox*>(comboBox)->setFocus();
}

void ContView::minusButton(QWidget *comboBox)
{
    QString temp = qobject_cast<QComboBox*>(comboBox)->currentText();
    qobject_cast<QComboBox*>(comboBox)->clearEditText();
    qobject_cast<QComboBox*>(comboBox)->removeItem(qobject_cast<QComboBox*>(comboBox)->findText(temp));
    qobject_cast<QComboBox*>(comboBox)->setFocus();
}


