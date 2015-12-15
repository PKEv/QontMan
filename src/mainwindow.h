#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "contview.h"
#include "contact.h"

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
     void on_QuitButton_clicked();
     void addButton();
     void update_rec();

private:
    Ui::MainWindow *ui;
    ContView *contView;

};

#endif // MAINWINDOW_H
