#include "diagram.h"
#include "ui_diagram.h"
#include <QVBoxLayout>

Diagram::Diagram(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Diagram)
{
    ui->setupUi(this);
    setWindowTitle(tr("Диаграмма контактов"));

    GraphWidget *widget = new GraphWidget;
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(widget);
}

Diagram::~Diagram()
{
    delete ui;
}
