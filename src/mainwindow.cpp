#include "mainwindow.h"
#include "ui_mainwindow.h"




MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    PlainDb::getInstance();

    myModel = new dataModel(qApp);
    //настраиваем отображение
    myModel->showAll();
    // подключаем модель
    ui->tableView->setModel( myModel );
    // скрываем служебные колонки
    ui->tableView->setColumnHidden(0,true);
    ui->tableView->setColumnHidden(1,true);

    ui->tableView->show();

    connect( ui->AddButton, SIGNAL(clicked()), this, SLOT(addButton()) );
    connect( ui->ViewButton, SIGNAL(clicked()), this, SLOT(viewButton()) );

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
    if (contView->getContact()->getId()==NULL)
    {
        PlainDb::getInstance()->addContact(contView->getContact());
    }
    else
    {
        PlainDb::getInstance()->updateContact(contView->getContact());
    }
    myModel->refresh();
}

void MainWindow::viewButton()
{
    int id = myModel->GetContactId(ui->tableView->currentIndex().row());

    Contact tcont = myModel->GetContact(id);
    contView = new ContView(&tcont);
    contView->show();
    connect(contView, SIGNAL(accepted()), this, SLOT(update_rec()));
}
