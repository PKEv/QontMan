#ifndef DIAGRAM_H
#define DIAGRAM_H

#include <QDialog>
#include "dia/graphwidget.h"

namespace Ui {
class Diagram;
}

class Diagram : public QDialog
{
    Q_OBJECT

public:
    explicit Diagram(QWidget *parent = 0);
    ~Diagram();

private:
    Ui::Diagram *ui;
    GraphWidget *widget;
};

#endif // DIAGRAM_H