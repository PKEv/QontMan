#include "passport.h"
#include "ui_passport.h"

#include <QVBoxLayout>

passport::passport(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::passport)
{
    ui->setupUi(this);
}

passport::passport(Contact * tcont, QWidget *parent):
    QDialog(parent),
    ui(new Ui::passport)
{
    this->setWindowFlags(Qt::Window
                         | Qt::WindowCloseButtonHint
                         | Qt::WindowSystemMenuHint
                         | Qt::CustomizeWindowHint
                         | Qt::WindowStaysOnTopHint
                         | Qt::WindowContextHelpButtonHint);


    ui->setupUi(this);
    cont = new Contact(*tcont);
    fill();

    QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    this->setSizePolicy(sizePolicy);  //QWidget::minimumSizeHint()
    setMinimumSize(QWidget::minimumSizeHint());
    adjustSize();

    this->setWindowTitle("Паспорт контакта");

    setWindowModality(Qt::ApplicationModal);
}
void passport::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    this->deleteLater();
}

void passport::hideEvent(QHideEvent *event)
{
    Q_UNUSED(event);
    this->deleteLater();
}

passport::~passport()
{
    delete ui;
    delete cont;
    delete shotNameLabel;
    if (telTitle)
        delete telTitle;
    if (telValue)
        delete telValue;
    if (faxTitle)
        delete faxTitle;
    if (faxValue)
        delete faxValue;
    if (httpTitle)
        delete httpTitle;
    if (httpValue)
        delete httpValue;
    if (emlTitle)
        delete emlTitle;
    if (emlValue)
        delete emlValue;
    if (adrTitle)
        delete adrTitle;
    if (adrValue)
        delete adrValue;
    if (zamTitle)
        delete zamTitle;
    if (zamValue)
        delete zamValue;
}

void passport::fill()
{
    QGridLayout *cardLayout = new QGridLayout(this);

    shotNameLabel = new QLabel(cont->getFullName());
    shotNameLabel->setTextFormat(Qt::PlainText);
    shotNameLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    QFont font = shotNameLabel->font();
    font.setBold(true);
    shotNameLabel->setFont(font);

    cardLayout->addWidget(shotNameLabel,0,0,1,3);

    if (!cont->getTel().isEmpty())
    {
        telTitle = new QLabel(tr("тел.:"));
        telTitle->setAlignment(Qt::AlignRight);
        QString end;
        QString str = cont->getTel();
        end = str.split(";",QString::SkipEmptyParts).join("<br>");
        telValue = new QLabel(end);
        telValue->setTextInteractionFlags(Qt::TextSelectableByMouse);
        telValue->setAlignment(Qt::AlignLeft);
        cardLayout->addWidget(telTitle,1,1);
        cardLayout->addWidget(telValue,1,2);
    }
    if (!cont->getFax().isEmpty())
    {
        faxTitle = new QLabel(tr("факс.:"));
        faxTitle->setAlignment(Qt::AlignRight);
        QString end;
        QString str = cont->getFax();
        end = str.split(";",QString::SkipEmptyParts).join("<br>");;
        faxValue = new QLabel(end);
        faxValue->setTextInteractionFlags(Qt::TextSelectableByMouse);
        faxValue->setAlignment(Qt::AlignLeft);
        cardLayout->addWidget(faxTitle,2,1);
        cardLayout->addWidget(faxValue,2,2);
    }
    if (!cont->getHttp().isEmpty())
    {
        httpTitle = new QLabel(tr("web:"));
        httpTitle->setAlignment(Qt::AlignRight);
        QString end;
        QString str = cont->getHttp();
        end = str.split(";",QString::SkipEmptyParts).join("<br>");
        httpValue = new QLabel(end);
        httpValue->setTextInteractionFlags(Qt::TextSelectableByMouse);
        httpValue->setTextFormat(Qt::RichText);
        httpValue->setOpenExternalLinks(true);
        httpValue->setAlignment(Qt::AlignLeft);
        cardLayout->addWidget(httpTitle,3,1);
        cardLayout->addWidget(httpValue,3,2);
    }
    if (!cont->getEmail().isEmpty())
    {
        emlTitle = new QLabel(tr("email:"));
        emlTitle->setAlignment(Qt::AlignRight);
        QString end;
        QString str = cont->getEmail();
        end = str.split(";",QString::SkipEmptyParts).join("<br>");
        emlValue = new QLabel(end);
        emlValue->setTextInteractionFlags(Qt::TextSelectableByMouse);
        emlValue->setOpenExternalLinks(true);
        emlValue->setTextFormat(Qt::RichText);
        emlValue->setAlignment(Qt::AlignLeft);
        cardLayout->addWidget(emlTitle,4,1);
        cardLayout->addWidget(emlValue,4,2);
    }
    if (!cont->getAdr().isEmpty())
    {
        adrTitle = new QLabel(tr("Адрес:"));
        adrTitle->setAlignment(Qt::AlignRight);
        QString end;
        QString str = cont->getAdr();
        end = str.split(";",QString::SkipEmptyParts).join("<br>");
        adrValue = new QLabel(end);
        adrValue->setTextInteractionFlags(Qt::TextSelectableByMouse);
        adrValue->setAlignment(Qt::AlignLeft);
        cardLayout->addWidget(adrTitle,5,1);
        cardLayout->addWidget(adrValue,5,2);
    }
    if (!cont->getZametka().isEmpty())
    {
        zamTitle = new QLabel(tr("Заметка:"));
        zamTitle->setAlignment(Qt::AlignRight);
        QString end = cont->getZametka().trimmed();
        zamValue = new QLabel(end);
        zamValue->setTextInteractionFlags(Qt::TextSelectableByMouse);
        zamValue->setWordWrap(true);
        zamValue->setAlignment(Qt::AlignLeft);
        cardLayout->addWidget(zamTitle,6,1);
        cardLayout->addWidget(zamValue,6,2);
    }
    if (cont->getIcon().isNull())
    {
        icon = new QLabel();
        QPixmap pic;
        if (cont->getTip()!=0)
            pic.load(":/img/pic/avatar/index.png");
        else
            pic.load(":/img/pic/avatar/index2.png");

        pic = pic.scaled(80,80,Qt::IgnoreAspectRatio, Qt::FastTransformation);
        icon->setPixmap(pic);
        icon->setAlignment(Qt::AlignCenter);
        //icon->setScaledContents(true);
        cardLayout->addWidget(icon,1,0,6,1);
    }
}
