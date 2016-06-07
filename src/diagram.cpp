#include "diagram.h"
#include "ui_diagram.h"
#include <QDesktopWidget>
#include <QDebug>


Diagram::Diagram(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Diagram)
{
    this->setWindowFlags(Qt::Window
                         | Qt::WindowCloseButtonHint
                         | Qt::WindowSystemMenuHint
                         | Qt::CustomizeWindowHint
                         | Qt::WindowStaysOnTopHint);
    ui->setupUi(this);

    setWindowTitle(tr("Диаграмма контактов"));

    widget = new GraphWidget();
    resize(QApplication::desktop()->availableGeometry().width()/2,
           QApplication::desktop()->availableGeometry().height()/2);
    mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(widget);
}

Diagram::Diagram(int id, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Diagram)
{
    this->setWindowFlags(Qt::Window
                         | Qt::WindowCloseButtonHint
                         | Qt::WindowSystemMenuHint
                         | Qt::CustomizeWindowHint
                         | Qt::WindowStaysOnTopHint);
    ui->setupUi(this);

    setWindowTitle(tr("Диаграмма контактов"));

    widget = new GraphWidget(id);
    resize(QApplication::desktop()->availableGeometry().width()/2,
           QApplication::desktop()->availableGeometry().height()/2);
    mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(widget);
}

void Diagram::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    this->deleteLater();
}

void Diagram::hideEvent(QHideEvent *event)
{
    Q_UNUSED(event);
    this->deleteLater();
}

Diagram::~Diagram()
{
    delete widget;
    delete mainLayout;
    delete ui;
}
