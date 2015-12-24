#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "contview.h"
#include "contact.h"
#include "datamodel.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();




private slots:
     void addButton();
     void viewButton();
     void update_rec();
     void deleteButton();
     void on_tableView_doubleClicked(const QModelIndex &index);
     void clearSeachString();
     void setSeachString(QString str);

private:
    Ui::MainWindow *ui;
    ContView *contView;
    dataModel *myModel;

};

#endif // MAINWINDOW_H
