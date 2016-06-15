
#include "contview.h"
#include "ui_contview.h"
#include "PlainDb.h"
#include <QFileDialog>

ContView::ContView(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ContView)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Window
                         | Qt::WindowCloseButtonHint
                         | Qt::WindowSystemMenuHint
                         | Qt::CustomizeWindowHint
                         | Qt::WindowStaysOnTopHint);
    cont = new Contact;
    Connect();
    Fill();
}

void ContView::ClearAvatar()
{
    QByteArray temp;
    temp.clear();
    cont->setIcon(temp);
    on_tipSlider_valueChanged(ui->tipSlider->value());
}

void ContView::SetAvatar()
{
    QString fileName = QFileDialog::getOpenFileName( this,
                                                    tr("Укажите файл с изображением"),
                                                    qApp->applicationDirPath(),
                                                    tr("Изображения (*.png *.jpg *.bmp)"));
    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly))
        return;
    QByteArray inByteArray = file.readAll();
    cont->setIcon(inByteArray);
    QPixmap image;
    image.loadFromData(inByteArray);
    ui->imgLabel->setFixedSize(size);
    ui->imgLabel->setPixmap(image.scaled(size, Qt::IgnoreAspectRatio, Qt::FastTransformation));
}

void ContView::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    this->deleteLater();
}
void ContView::hideEvent(QHideEvent * event)
{
    Q_UNUSED(event);
    this->deleteLater();
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
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(autoFullName()));

    openAction = new QAction(QIcon(),tr("Назначить аватар"), this);
    clearAction = new QAction(QIcon(),tr("Удалить аватар"), this);
    connect(clearAction, SIGNAL(triggered(bool)), this, SLOT(ClearAvatar()) );
    connect(openAction, SIGNAL(triggered(bool)), this, SLOT(SetAvatar()) );
}

ContView::ContView(Contact * tcont, QWidget *parent): ui(new Ui::ContView), QDialog(parent)
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
    delete signalMapper;
    delete signalMapper2;
}

void ContView::Fill()
{
    ui->lineEdit->setText(cont->getFullName());
    SetItiems(ui->comboBox, cont->getTel());
    SetItiems(ui->comboBox_2, cont->getFax());
    SetItiems(ui->comboBox_3, cont->getEmail());
    SetItiems(ui->comboBox_4, cont->getHttp());
    SetItiems(ui->comboBox_5, cont->getAdr());

    ui->name1Edit->setText(cont->getName1());
    ui->name2Edit->setText(cont->getName2());
    ui->name3Edit->setText(cont->getName3());
    ui->lineEdit->setText(cont->getFullName());
    ui->textEdit->setText(cont->getZametka());
    ui->tipSlider->setValue(cont->getTip());
    on_tipSlider_valueChanged(cont->getTip());
    ui->lineEdit->setPlaceholderText(tr("Имя отображаемое в базе"));

    SetupUpLevel();
}

void ContView::SetupUpLevel()
{
    ui->accessoryComboBox->clear();
    ui->accessoryComboBox->addItem("0:нет");                          // нулевой
    std::vector<QString> vec;
    PlainDb::getInstance()->GetAcsList(vec, cont->getId());
    for(auto iter = vec.begin(); iter!=vec.end(); ++iter)
    {
        ui->accessoryComboBox->addItem(*iter);
    }
    if (cont->getUpLevel() == 0)
    {
        ui->accessoryComboBox->setCurrentIndex(0);
    }
    else
    {
        for(int i = 1; i < ui->accessoryComboBox->count(); i++)
        {
            QString find = QString::number(cont->getUpLevel()) + ":";
            if (ui->accessoryComboBox->itemText(i).indexOf(find) != -1)
            {
                ui->accessoryComboBox->setCurrentIndex(i);
                break;
            }
        }
    }
}

int ContView::GetUpLevel()
{
    QString temp = ui->accessoryComboBox->currentText();
    int i = temp.indexOf(":");
    return temp.mid(0,i).toInt();
}

Contact *ContView::getContact()
{
    return cont;
}

void ContView::on_buttonBox_clicked(QAbstractButton *button)
{
    if (ui->buttonBox->standardButton(button) == QDialogButtonBox::Ok)
    {
        bool tipChanged {false};
        if ((cont->getTip() != ui->tipSlider->value()) && cont->getId()!=0)
            tipChanged = true;
        // если вдруг забыли нажать +
        ui->plusButton_1->click();
        ui->plusButton_2->click();
        ui->plusButton_3->click();
        ui->plusButton_4->click();
        ui->plusButton_5->click();

        //Проверяем что заполнено поле Представления контакта
        if (ui->lineEdit->text().trimmed().isEmpty())
            ui->pushButton->click();

        // заполняем структуру для передачи
        cont->setFullName(ui->lineEdit->text());
        cont->setTel(GetItiems(ui->comboBox));
        cont->setFax(GetItiems(ui->comboBox_2));
        cont->setEmail(GetItiems(ui->comboBox_3));
        cont->setHttp(GetItiems(ui->comboBox_4));
        cont->setAdr(GetItiems(ui->comboBox_5));
        cont->setZametka(ui->textEdit->toPlainText());
        cont->setName1(ui->name1Edit->text());
        cont->setName2(ui->name2Edit->text());
        cont->setName3(ui->name3Edit->text());
        cont->setTip(ui->tipSlider->value());
        cont->setDate(QDate::currentDate().toString("dd.MM.yyyy"));

        cont->setUpLevel(GetUpLevel());
        if (tipChanged == false)
        {
            if (cont->getId()==0)
            {
                PlainDb::getInstance()->addContact(cont);
            }
            else
            {
                PlainDb::getInstance()->updateContact(cont);
            }
        }
        else
        {
            PlainDb::getInstance()->changeContactTip(cont);

        }
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
        if (!comboBox->itemText(i).isEmpty())
            temp += comboBox->itemText(i) + ";";
    }
    return temp;
}

void ContView::SetItiems(QComboBox * comboBox, QString str)
{

    QStringList temp = str.split(";",QString::SkipEmptyParts);
    comboBox->addItems(temp);
}

void ContView::on_tipSlider_valueChanged(int value)
{
    if (value==0)
    {
        ui->tipLabel->setText(tr("Фирма"));
        ui->name3Edit->setVisible(false);
        ui->name1Edit->setPlaceholderText(tr("Форма собственности"));
        ui->name2Edit->setPlaceholderText(tr("Наименование"));

        if (!cont->getIcon().isEmpty()) // загрузка изображения если нет в базе
        {

        }
    }
    else
    {
        ui->tipLabel->setText(tr("Контактное лицо"));
        ui->name3Edit->setVisible(true);
        ui->name1Edit->setPlaceholderText(tr("Имя"));
        ui->name2Edit->setPlaceholderText(tr("Фамилия"));
        ui->name3Edit->setPlaceholderText(tr("Отчество"));

        if (!cont->getIcon().isEmpty()) // загрузка изображения если нет в базе
        {
            QPixmap image;
            image.loadFromData(cont->getIcon());
            ui->imgLabel->setFixedSize(size);
            ui->imgLabel->setPixmap(image.scaled(size, Qt::IgnoreAspectRatio, Qt::FastTransformation));
        }
    }
    if (cont->getIcon().isEmpty()) // загрузка изображения если нет в базе
    {
        QFile file;
        if (value==0)
            file.setFileName(":/img/pic/avatar/index2.png");
        else
            file.setFileName(":/img/pic/avatar/index.png");

        if (!file.open(QIODevice::ReadOnly))
            return;
        QByteArray inByteArray = file.readAll();
        QPixmap image;
        image.loadFromData(inByteArray);
        ui->imgLabel->setFixedSize(size);
        ui->imgLabel->setPixmap(image.scaled(size, Qt::IgnoreAspectRatio, Qt::FastTransformation));
    }
}

void ContView::autoFullName()
{
    auto name1 = ui->name1Edit->text().trimmed();
    auto name2 = ui->name2Edit->text().trimmed();
    auto name3 = ui->name3Edit->text().trimmed();

    if (ui->tipSlider->value()==0) // фирма
    {
        if (!name1.isEmpty() && !name2.isEmpty())
            ui->lineEdit->setText(name2 + ", " + name1);
        else if (!name1.isEmpty() && name2.isEmpty())
            ui->lineEdit->setText(name2);
        else
            ui->lineEdit->setText(tr("Не введено!!!"));
    }
    else //физ лицо
    {
        if (!name1.isEmpty() && !name2.isEmpty() && !name3.isEmpty())
            ui->lineEdit->setText(name2 + ", "+name1 + " " + name3);
        else if (name2.isEmpty() && !name1.isEmpty() && !name3.isEmpty())
                ui->lineEdit->setText(name1 + " " + name3);
        else if (name1.isEmpty() && !name1.isEmpty() && name3.isEmpty())
                ui->lineEdit->setText(name1);
        else
             ui->lineEdit->setText((name2 + " "+name1 + " " + name3).trimmed());
    }
}

void ContView::on_imgLabel_customContextMenuRequested(const QPoint &pos)
{
    QMenu menu(this);
    menu.addAction(openAction);
    menu.addSeparator();
    menu.addAction(clearAction);
    menu.exec(mapToParent(pos) + ui->imgLabel->pos());
}
