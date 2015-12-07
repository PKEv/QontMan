#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "datamodel.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    PlainDb::getInstance();

    dataModel *myModel = new dataModel(qApp);
    //настраиваем отображение
    myModel->showAll();
    // подключаем модель
    ui->tableView->setModel( myModel );
    // скрываем служебные колонки
    ui->tableView->setColumnHidden(0,true);
    ui->tableView->setColumnHidden(1,true);

    ui->tableView->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_QuitButton_clicked()
{
    qApp->quit();
}
