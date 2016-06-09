#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "contview.h"
#include "passport.h"
#include "contact.h"
#include "datamodel.h"
#include "treemodel.h"
#include "diagram.h"
#include "vcard.h"
#include <QSystemTrayIcon>
#include <QCloseEvent>
#include <QSettings>

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
    void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;

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
     void ExportCont();
     void ImportCont();

private:
    void setupMenu();
    Ui::MainWindow *ui = nullptr;
    ContView *contView = nullptr;
    dataModel *myModel = nullptr;
    TreeModel *myTreeModel = nullptr;
    Diagram *myDia = nullptr;
    passport *passView = nullptr;

    void createTrayIcon();
    void readSettings();
    void writeSettings();
    QAction *quitAction = nullptr;
    QAction *restoreAction = nullptr;
    QAction *diaAction = nullptr;
    QAction *addAction = nullptr;
    QAction *editAction = nullptr;
    QAction *remAction = nullptr;
    QAction *exportCont = nullptr;
    QAction *importCont = nullptr;

    QSystemTrayIcon *trayIcon = nullptr;
    QMenu *trayIconMenu = nullptr;
    QSettings *settings = nullptr;
};

#endif // MAINWINDOW_H
