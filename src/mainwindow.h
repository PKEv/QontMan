#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "contview.h"
#include "passport.h"
#include "contact.h"
#include "datamodel.h"
#include "treemodel.h"
#include "diagram.h"
#include <QSystemTrayIcon>
#include <QCloseEvent>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


protected:
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

private slots:
     void addButton();
     void iconActivated(QSystemTrayIcon::ActivationReason reason);
     void viewButton();
     void showPass();
     void update_rec();
     void deleteButton();
     void clearSeachString();
     void setSeachString(QString str);
     void filterByTree(const QModelIndex & index);
     void showDiagram();

private:
    Ui::MainWindow *ui = nullptr;
    ContView *contView = nullptr;
    dataModel *myModel = nullptr;
    TreeModel *myTreeModel = nullptr;
    Diagram *myDia = nullptr;
    passport *passView = nullptr;

    void createTrayIcon();
    QAction *quitAction;
    QAction *restoreAction;


    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;
};

#endif // MAINWINDOW_H
