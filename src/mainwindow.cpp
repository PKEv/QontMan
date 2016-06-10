#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>

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

    myModel = new dataModel();
    //настраиваем отображение
    myModel->showAll();
    // подключаем модель
    ui->tableView->setModel( myModel );
    ui->tableView->show();
    // скрываем служебные колонки
    ui->tableView->setColumnHidden(0, true);
    ui->tableView->setColumnHidden(1, true);

    myTreeModel = new TreeModel(this);
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

    const QString path = qApp->applicationDirPath() + "/settings.ini";
    settings = new QSettings(path, QSettings::IniFormat);
}

void MainWindow::showEvent(QShowEvent *event)
{
    //Q_UNUSED(event);
    QMainWindow::showEvent(event);
    readSettings();
}

void MainWindow::setupMenu()
{
    diaAction = new QAction(QIcon(":/img/pic/dia.ico"), tr("Диаграмма"), this);
    addAction = new QAction(QIcon(":/img/pic/add.ico"), tr("Добавить"), this);
    editAction = new QAction(QIcon(":/img/pic/edit.ico"), tr("Редактировать"), this);
    remAction = new QAction(QIcon(":/img/pic/rem.ico"), tr("Удалить"), this);
    exportCont = new QAction(QIcon(":/img/pic/export.ico"), tr("Экспорт"), this);
    importCont = new QAction(QIcon(":/img/pic/import.ico"), tr("Импорт"), this);

    diaAction->setShortcut( QKeySequence(Qt::CTRL + Qt::Key_D));
    addAction->setShortcut( QKeySequence(Qt::CTRL + Qt::Key_N));

    //соединяем
    connect(diaAction, SIGNAL(triggered()), this, SLOT(showDiagram()));
    connect(addAction, SIGNAL(triggered()), this, SLOT(addButton()));
    connect(editAction, SIGNAL(triggered()), this, SLOT(viewButton()));
    connect(remAction, SIGNAL(triggered()), this, SLOT(deleteButton()));
    connect(exportCont, SIGNAL(triggered()), this, SLOT(ExportCont()));
    connect(importCont, SIGNAL(triggered()), this, SLOT(ImportCont()));

    //настройка панели
    ui->toolBar->addAction(diaAction);
    ui->toolBar->addSeparator();
    ui->toolBar->addAction(addAction);
    ui->toolBar->addAction(editAction);
    ui->toolBar->addAction(remAction);
    ui->toolBar->addSeparator();
    ui->toolBar->addAction(exportCont);
    ui->toolBar->addAction(importCont);

    //настройка меню
    QMenu *fileMenu = new QMenu(tr("Меню"));
    fileMenu->addAction(exportCont);
    fileMenu->addAction(importCont);
    fileMenu->addSeparator();
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
    writeSettings();
    //delete ui;
   /* delete */ myTreeModel->deleteLater();
    /*delete */ myModel->deleteLater();
    delete ui;
    delete quitAction;
    delete restoreAction;
    delete diaAction;
    delete addAction;
    delete editAction;
    delete remAction;    
    delete settings;
    delete trayIcon;
    delete trayIconMenu;
}

void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::Trigger)
    {
        if (!this->isVisible())
        {
            this->show();
            this->activateWindow();
            readSettings();
        }
        else
            this->close();
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (trayIcon->isVisible())
    {
        hide();
        event->ignore();
    }

    writeSettings();

}
void MainWindow::readSettings()
{
    settings->sync();
    QPoint pos = settings->value("pos", QPoint(200, 200)).toPoint();
    QSize size = settings->value("size", QSize(800, 400)).toSize();
    resize(size);
    ui->tableView->setColumnWidth(2, settings->value("colum1", 200).toInt());
    ui->tableView->setColumnWidth(3, settings->value("colum2", 100).toInt());
    ui->tableView->setColumnWidth(4, settings->value("colum3", 100).toInt());
    ui->tableView->setColumnWidth(5, settings->value("colum4", 100).toInt());
    ui->tableView->setColumnWidth(6, settings->value("colum5", 100).toInt());
    ui->tableView->setColumnWidth(7, settings->value("colum6", 100).toInt());

    move(pos);
}

void MainWindow::writeSettings()
{
    settings->setValue("pos", pos());
    settings->setValue("size", size());

    settings->setValue("colum1", ui->tableView->columnWidth(2));
    settings->setValue("colum2", ui->tableView->columnWidth(3));
    settings->setValue("colum3", ui->tableView->columnWidth(4));
    settings->setValue("colum4", ui->tableView->columnWidth(5));
    settings->setValue("colum5", ui->tableView->columnWidth(6));
    settings->setValue("colum6", ui->tableView->columnWidth(7));

    settings->sync();
}


void MainWindow::addButton()
{
    contView = new ContView(this);
    contView->show();

    connect(contView, SIGNAL(finished(int)), contView, SLOT(deleteLater()));
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
    passView = new passport(&tcont, this);
    passView->show();
    connect(passView, SIGNAL(finished(int)), passView, SLOT(deleteLater()));

    setFocus();
}

void MainWindow::viewButton()
{
    if (ui->tableView->currentIndex().row()==-1)
        return;
    int id = myModel->GetContactId(ui->tableView->currentIndex().row());

    Contact tcont = myModel->GetContact(id);
    contView = new ContView(&tcont, this);
    contView->exec();

    connect(contView, SIGNAL(finished(int)), contView, SLOT(deleteLater()));
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
    //QModelIndex ui->treeView->currentIndex()
    NodeInfo* nodeInfo = static_cast<NodeInfo*>(ui->treeView->currentIndex().internalPointer());
    int id  = 0;
    if (nodeInfo != NULL && nodeInfo->parent != NULL)
        id = nodeInfo->cont.getId();
    myDia = new Diagram(id, this);
    myDia->exec();
    connect(myDia, SIGNAL(finished(int)), myDia, SLOT(deleteLater()));
    qDebug() << "Diagram show";
}

void MainWindow::createTrayIcon()
{
    quitAction = new QAction(QIcon(), tr("Выход"), this);
    quitAction->setShortcut( QKeySequence(Qt::CTRL + Qt::Key_Q));
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

    restoreAction = new QAction(QIcon(), tr("Открыть"), this);
    connect(restoreAction, &QAction::triggered, this, &QWidget::show);

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
void MainWindow::ExportCont()
{
    if (ui->tableView->currentIndex().row()==-1)
        return;
    int id = myModel->GetContactId(ui->tableView->currentIndex().row());

    Contact tcont = myModel->GetContact(id);

    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Экспорт контакта: \"") + tcont.getFullName() +"\"",
                                                    qApp->applicationDirPath() + QDir::separator() + tcont.getFullName(),
                                                    tr("VCard (*.vcf)"));

    VCard::Export(fileName, &tcont);
}

void MainWindow::ImportCont()
{
    QString fileName = QFileDialog::getOpenFileName( this,
                                                    tr("Импорт контакта"),
                                                    qApp->applicationDirPath(),
                                                    tr("VCard (*.vcf)"));
    Contact tcont;
    VCard::Import(fileName, &tcont);

    contView = new ContView(&tcont, this);
    contView->exec();

    connect(contView, SIGNAL(finished(int)), contView, SLOT(deleteLater()));
    connect(contView, SIGNAL(accepted()), this, SLOT(update_rec()));
}

