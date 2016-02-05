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

    myTreeModel = new TreeModel();
    ui->treeView->setModel(myTreeModel);
    ui->treeView->show();
    ui->toolBar->addAction(QIcon(),tr("Диаграмма"),this,SLOT(showDiagram()));

    connect( ui->AddButton, SIGNAL(clicked()), this, SLOT(addButton()) );
    connect( ui->ViewButton, SIGNAL(clicked()), this, SLOT(viewButton()) );
    connect( ui->DeleteButton, SIGNAL(clicked()), this, SLOT(deleteButton()) );
    connect(ui->clearSearchButton, SIGNAL(clicked()),this, SLOT(clearSeachString()) );
    connect(ui->SeachString, SIGNAL(textChanged(QString)),this, SLOT(setSeachString(QString)));
    connect(ui->treeView, SIGNAL(clicked(QModelIndex)), this, SLOT(filterByTree(QModelIndex)));

}

MainWindow::~MainWindow()
{
    delete myDia;
    delete myTreeModel;
    delete myModel;
    delete contView;
    delete ui;
}

void MainWindow::addButton()
{
    contView = new ContView();
    contView->show();
    connect(contView, SIGNAL(accepted()), this, SLOT(update_rec()));
}

void MainWindow::update_rec()
{    
    myModel->refresh();
}

void MainWindow::viewButton()
{
    if (ui->tableView->currentIndex().row()==-1)
        return;
    int id = myModel->GetContactId(ui->tableView->currentIndex().row());

    Contact tcont = myModel->GetContact(id);
    contView = new ContView(&tcont);
    contView->show();

    connect(contView, SIGNAL(accepted()), this, SLOT(update_rec()));
}

void MainWindow::deleteButton()
{
    if (ui->tableView->currentIndex().row()==-1)
        return;
    myModel->deleteContact(ui->tableView->currentIndex().row());
    myModel->refresh();
}

void MainWindow::on_tableView_doubleClicked(const QModelIndex &index)
{
    int id = myModel->GetContactId(index.row());

    Contact tcont = myModel->GetContact(id);
    contView = new ContView(&tcont);
    contView->show();

    connect(contView, SIGNAL(accepted()), this, SLOT(update_rec()));
}

void MainWindow::clearSeachString()
{
    ui->SeachString->clear();
}

void MainWindow::setSeachString(QString str)
{
    myModel->setSeachString(str);
    myModel->refresh();
}
void MainWindow::filterByTree(const QModelIndex & index)
{
    QStringList list =  myTreeModel->getDescListId(index);
    myModel->SetFilterByListId(list);
    myModel->refresh();
}

void MainWindow::showDiagram()
{
    myDia = new Diagram;
    myDia->show();
}

