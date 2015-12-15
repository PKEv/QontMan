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

    connect( ui->AddButton, SIGNAL(clicked()), this, SLOT(addButton()) );
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_QuitButton_clicked()
{
    qApp->quit();
}


void MainWindow::addButton()
{
    contView = new ContView();
    contView->show();
    connect(contView, SIGNAL(accepted()), this, SLOT(update_rec()));
}

void MainWindow::update_rec()
{
    PlainDb::getInstance()->addContact(contView->getContact());
}
