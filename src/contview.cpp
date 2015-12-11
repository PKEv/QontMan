#include "contview.h"
#include "ui_contview.h"

ContView::ContView(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ContView)
{
    ui->setupUi(this);
}

ContView::~ContView()
{
    delete ui;
}
