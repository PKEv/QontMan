#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    PlainDb::getInstance();
    //настройка отображения трея
    createTrayIcon();
    trayIcon->show();

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
    connect( ui->tableView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(showPass())) ;
    connect(ui->clearSearchButton, SIGNAL(clicked()),this, SLOT(clearSeachString()) );
    connect(ui->SeachString, SIGNAL(textChanged(QString)),this, SLOT(setSeachString(QString)));
    connect(ui->treeView, SIGNAL(clicked(QModelIndex)), this, SLOT(filterByTree(QModelIndex)));
    connect(trayIcon, &QSystemTrayIcon::activated, this, iconActivated);

    setWindowTitle(tr("Телефонная книга"));
    qDebug() << "MainWindow constr";
}

MainWindow::~MainWindow()
{
    if (myDia != nullptr)
        delete myDia;
    delete myTreeModel;
    delete myModel;
    delete contView;
    delete ui;
    delete passView;
}

void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::Trigger)
    {
        if (!this->isVisible())
        {
            this->showNormal();
            this->activateWindow();
        }
        else
            this->hide();
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (trayIcon->isVisible())
    {
        hide();
        event->ignore();
    }
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
void MainWindow::showPass()
{
    if (ui->tableView->currentIndex().row()==-1)
        return;
    int id = myModel->GetContactId(ui->tableView->currentIndex().row());

    Contact tcont = myModel->GetContact(id);
    passView = new passport(&tcont);
    passView->show();
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
    QMessageBox msgBox(QMessageBox::Question,
                       tr("Подтверждение"),
                       tr("Вы уверены что хотите удалить?"),
                       QMessageBox::Yes | QMessageBox::No );
    msgBox.exec();
    if (msgBox.result() == QMessageBox::No )
        return;
    myModel->deleteContact(ui->tableView->currentIndex().row());
    myModel->refresh();
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
    myDia = new Diagram();
    myDia->exec();
    qDebug() << "Diagram show";
}

void MainWindow::createTrayIcon()
{

    quitAction = new QAction(tr("Выход"), this);
    connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);

    restoreAction = new QAction(tr("Открыть"), this);
    connect(restoreAction, &QAction::triggered, this, &QWidget::showNormal);


    trayIconMenu = new QMenu(this);

    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);

    QIcon icon = QIcon(":/img/pic/tel.png");
    trayIcon->setIcon(icon);

    trayIcon->setToolTip("Телефонная книга");
}
