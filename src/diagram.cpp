#include "diagram.h"
#include "ui_diagram.h"


Diagram::Diagram(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Diagram)
{
    ui->setupUi(this);
    setWindowTitle(tr("Диаграмма контактов"));

    widget = new GraphWidget;
    mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(widget);
}

Diagram::~Diagram()
{
    delete widget;
    delete mainLayout;
    delete ui;
}
