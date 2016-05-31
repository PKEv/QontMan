#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include "settings.h"

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
    // наименования столбцов
    myModel->setHeaderData(2, Qt::Horizontal, tr("Имя"), Qt::DisplayRole);
    myModel->setHeaderData(3, Qt::Horizontal, tr("Телефон"), Qt::DisplayRole);
    myModel->setHeaderData(4, Qt::Horizontal, tr("Факс"), Qt::DisplayRole);
    myModel->setHeaderData(5, Qt::Horizontal, tr("Эл.почта"), Qt::DisplayRole);
    myModel->setHeaderData(6, Qt::Horizontal, tr("Сайт"), Qt::DisplayRole);
    myModel->setHeaderData(7, Qt::Horizontal, tr("Заметка"), Qt::DisplayRole);
    // подключаем модель
    ui->tableView->setModel( myModel );
    // скрываем служебные колонки
    ui->tableView->setColumnHidden(0, true);
    ui->tableView->setColumnHidden(1, true);

    ui->tableView->show();

    myTreeModel = new TreeModel();
    ui->treeView->setModel(myTreeModel);
    ui->treeView->show();

    setupMenu();

    connect(ui->tableView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(showPass())) ;
    connect(ui->clearSearchButton, SIGNAL(clicked()),this, SLOT(clearSeachString()) );
    connect(ui->SeachString, SIGNAL(textChanged(QString)),this, SLOT(setSeachString(QString)));
    connect(ui->treeView, SIGNAL(clicked(QModelIndex)), this, SLOT(filterByTree(QModelIndex)));
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));

    ui->clearSearchButton->setShortcut(Qt::Key_Escape);

    setWindowTitle(tr("Телефонная книга"));
}
void MainWindow::setupMenu()
{
    diaAction = new QAction(QIcon(":/img/pic/dia.ico"), tr("Диаграмма"), this);
    addAction = new QAction(QIcon(":/img/pic/add.ico"), tr("Добавить"), this);
    editAction = new QAction(QIcon(":/img/pic/edit.ico"), tr("Редактировать"), this);
    remAction = new QAction(QIcon(":/img/pic/rem.ico"), tr("Удалить"), this);

    diaAction->setShortcut( QKeySequence(Qt::CTRL + Qt::Key_D));
    addAction->setShortcut( QKeySequence(Qt::CTRL + Qt::Key_N));

    //соединяем
    connect(diaAction, SIGNAL(triggered()), this, SLOT(showDiagram()));
    connect(addAction, SIGNAL(triggered()), this, SLOT(addButton()));
    connect(editAction, SIGNAL(triggered()), this, SLOT(viewButton()));
    connect(remAction, SIGNAL(triggered()), this, SLOT(deleteButton()));

    //настройка панели
    ui->toolBar->addAction(diaAction);
    ui->toolBar->addSeparator();
    ui->toolBar->addAction(addAction);
    ui->toolBar->addAction(editAction);
    ui->toolBar->addAction(remAction);

    //настройка меню
    QMenu *fileMenu = new QMenu(tr("Меню"));
    fileMenu->addAction(quitAction);

    QMenu *actionMenu = new QMenu( tr("Операции") );
    actionMenu->addAction(addAction);
    actionMenu->addAction(editAction);
    actionMenu->addAction(remAction);

    QMenu *diaMenu = new QMenu( tr("Диаграмма") );
    diaMenu->addAction(diaAction);

    //отображаем
    ui->menuBar->addMenu(fileMenu);
    ui->menuBar->addMenu(actionMenu);
    ui->menuBar->addMenu(diaMenu);

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
    delete quitAction;
    delete restoreAction;
    delete diaAction;
    delete addAction;
    delete editAction;
    delete remAction;
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
    setFocus();
}

void MainWindow::viewButton()
{
    if (ui->tableView->currentIndex().row()==-1)
        return;
    int id = myModel->GetContactId(ui->tableView->currentIndex().row());

    Contact tcont = myModel->GetContact(id);
    contView = new ContView(&tcont);
    contView->exec();

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
    quitAction = new QAction(QIcon(), tr("Выход"), this);
    quitAction->setShortcut( QKeySequence(Qt::CTRL + Qt::Key_Q));
    connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);

    restoreAction = new QAction(QIcon(), tr("Открыть"), this);
    connect(restoreAction, &QAction::triggered, this, &QWidget::showNormal);

    // cоставляем меню
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);

    //иконка
    QIcon icon = QIcon(":/img/pic/tel.png");
    trayIcon->setIcon(icon);

    trayIcon->setToolTip(tr("Телефонная книга"));
}
