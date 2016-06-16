#ifndef CONTVIEW_H
#define CONTVIEW_H

#include <QDialog>
#include <QComboBox>
#include <QSignalMapper>
#include <QDate>
#include <QMenu>
#include "contact.h"
#include <qabstractbutton.h>

namespace Ui {
class ContView;
}

class ContView : public QDialog
{
    Q_OBJECT

public:
    explicit ContView(QWidget *parent = 0);
    ContView(Contact * tcont, QWidget *parent = 0);
    ~ContView();
    Contact* getContact();

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);
    void plusButton(QWidget *comboBox);
    void minusButton(QWidget *comboBox);
    void on_tipSlider_valueChanged(int value);
    void autoFullName();
    void ClearAvatar();
    void SetAvatar();

    void on_imgLabel_customContextMenuRequested(const QPoint &pos);

protected:
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;
    void hideEvent(QHideEvent * event) Q_DECL_OVERRIDE;

private:
    Ui::ContView *ui ;//= nullptr;
    void Fill();                                                // заполнение формы
    void Connect();                                             // настройка связей
    void LoadDefImage(int value);                                        // загрузка стандартной картинки
    QString GetItiems(QComboBox * comboBox);                    // чтение элемента формы в строку
    void SetItiems(QComboBox * comboBox, QString str);          // заполнение элемента данными из строки
    Contact * cont = nullptr;
    QSignalMapper *signalMapper;
    QSignalMapper *signalMapper2;    
    void SetupUpLevel();
    int GetUpLevel();
    QSize size = QSize(50,50);
    QAction *openAction = nullptr;
    QAction *clearAction = nullptr;
};

#endif // CONTVIEW_H
