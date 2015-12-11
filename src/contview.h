#ifndef CONTVIEW_H
#define CONTVIEW_H

#include <QDialog>

namespace Ui {
class ContView;
}

class ContView : public QDialog
{
    Q_OBJECT

public:
    explicit ContView(QWidget *parent = 0);
    ~ContView();

private:
    Ui::ContView *ui;
};

#endif // CONTVIEW_H
