#include "passport.h"
#include "ui_passport.h"

#include <QVBoxLayout>

passport::passport(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::passport)
{
    ui->setupUi(this);
}

passport::passport(Contact * tcont)
{
    ui->setupUi(this);
    cont = new Contact(*tcont);
    fill();

    QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    //sizePolicy.setHorizontalStretch(0);
    //sizePolicy.setVerticalStretch(0);
    this->setSizePolicy(sizePolicy);  //QWidget::minimumSizeHint()
    setMinimumSize(QWidget::minimumSizeHint());
    adjustSize();

    this->setWindowTitle("Паспорт контакта");
}

passport::~passport()
{
    delete ui;
    delete cont;
    delete shotNameLabel;
    delete telTitle;
    delete telValue;
    delete faxTitle;
    delete faxValue;
    delete httpTitle;
    delete httpValue;
    delete emlTitle;
    delete emlValue;
    delete adrTitle;
    delete adrValue;
    delete zamTitle;
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
        cardLayout->addWidget(adrTitle,5,1);
        cardLayout->addWidget(adrValue,5,2);
    }
    if (!cont->getZametka().isEmpty())
    {
        zamTitle = new QLabel(tr("Заметка:"));
        zamTitle->setAlignment(Qt::AlignRight);
        QString end = cont->getZametka();
        zamValue = new QLabel(end);
        zamValue->setTextInteractionFlags(Qt::TextSelectableByMouse);
        cardLayout->addWidget(zamTitle,6,1);
        cardLayout->addWidget(zamValue,6,2);
    }


}
