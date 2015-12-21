
#include "contview.h"
#include "ui_contview.h"


ContView::ContView(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ContView)
{
    ui->setupUi(this);
    cont = new Contact;
    Connect();
    Fill();
}

void ContView::Connect()
{
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

ContView::ContView(Contact * tcont): ui(new Ui::ContView)
{
    ui->setupUi(this);
    cont = new Contact(*tcont);
    Connect();
    Fill();
}


ContView::~ContView()
{
    delete cont;
    delete ui;
}

void ContView::Fill()
{
    ui->lineEdit->setText(cont->getFullName());
    SetItiems(ui->comboBox, cont->getTel());
    SetItiems(ui->comboBox_2, cont->getFax());
    SetItiems(ui->comboBox_3, cont->getAdr());
    SetItiems(ui->comboBox_4, cont->getEmail());
    SetItiems(ui->comboBox_5, cont->getHttp());

    ui->name1Edit->setText(cont->getName1());
    ui->name2Edit->setText(cont->getName2());
    ui->name3Edit->setText(cont->getName3());
    ui->lineEdit->setText(cont->getFullName());
    ui->textEdit->setText(cont->getZametka());
    ui->tipSlider->setValue(cont->getTip());
    on_tipSlider_valueChanged(cont->getTip());
    ui->label_8->setText(cont->getDate());
    ui->lineEdit->setPlaceholderText(tr("Имя отображаемое в базе"));
}

Contact *ContView::getContact()
{
    return cont;
}

void ContView::on_buttonBox_clicked(QAbstractButton *button)
{
    if (ui->buttonBox->standardButton(button) == QDialogButtonBox::Ok)
    {
        // если вдруг забыли нажать +
        ui->plusButton_1->click();
        ui->plusButton_2->click();
        ui->plusButton_3->click();
        ui->plusButton_4->click();
        ui->plusButton_5->click();

        // заполняем структуру для передачи
        cont->setFullName(ui->lineEdit->text());
        cont->setTel(GetItiems(ui->comboBox));
        cont->setFax(GetItiems(ui->comboBox_2));
        cont->setAdr(GetItiems(ui->comboBox_3));
        cont->setEmail(GetItiems(ui->comboBox_4));
        cont->setHttp(GetItiems(ui->comboBox_5));
        cont->setZametka(ui->textEdit->toPlainText());
        cont->setName1(ui->name1Edit->text());
        cont->setName2(ui->name2Edit->text());
        cont->setName3(ui->name3Edit->text());
        cont->setTip(ui->tipSlider->value());
        cont->setDate(QDate::currentDate().toString("dd.MM.yyyy"));
        cont->setUpLevel(0);
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

QString ContView::GetItiems(QComboBox * comboBox)
{
    QString temp = "";
    for(int i=0; i < comboBox->count(); i++)
    {
        temp += comboBox->itemText(i) + ";";
    }
    return temp;
}

void ContView::SetItiems(QComboBox * comboBox, QString str)
{
    int t = str.indexOf(";");
    QString temp;
    while (t>0)
    {
        temp = str;
        temp.remove(t,temp.length());
        comboBox->addItem(temp);
        temp = str;
        str.remove(0,t+1); // del ";"
        t = str.indexOf(";");
    }
    if (!str.isEmpty())
    {
        comboBox->addItem(str);
    }
}

void ContView::on_tipSlider_valueChanged(int value)
{
    if (value==0)
    {
        ui->tipLabel->setText(tr("Фирма"));
        ui->name3Edit->setVisible(false);
        ui->name1Edit->setPlaceholderText(tr("Форма собственности"));
        ui->name2Edit->setPlaceholderText(tr("Наименование"));
    }
    else
    {
        ui->tipLabel->setText(tr("Контактное лицо"));
        ui->name3Edit->setVisible(true);
        ui->name1Edit->setPlaceholderText(tr("Имя"));
        ui->name2Edit->setPlaceholderText(tr("Фамилия"));
        ui->name3Edit->setPlaceholderText(tr("Отчество"));
    }
}
