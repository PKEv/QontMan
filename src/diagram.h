#ifndef DIAGRAM_H
#define DIAGRAM_H

#include <QDialog>
#include <QVBoxLayout>
#include "dia/graphwidget.h"

namespace Ui {
class Diagram;
}

class Diagram : public QDialog
{
    Q_OBJECT

public:
    explicit Diagram(QWidget *parent = 0);
    Diagram(int id, QWidget *parent = 0);
    ~Diagram();

protected:
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;
    void hideEvent(QHideEvent * event) Q_DECL_OVERRIDE;

private:
    Ui::Diagram *ui = nullptr;
    GraphWidget *widget = nullptr;
    QVBoxLayout *mainLayout = nullptr;
};

#endif // DIAGRAM_H
