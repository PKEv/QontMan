#ifndef DATAMODEL_H
#define DATAMODEL_H

#include "PlainDb.h"
#include <QSqlQueryModel>
#include <QString>
#include "contact.h"

class dataModel : public QSqlQueryModel
{
    Q_OBJECT
public:
    explicit dataModel(QObject *parent=0);
    ~dataModel();

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    void refresh();
    int GetContactId(int row);
    Contact GetContact(int id);
    void deleteContact(int row);
    void setSeachString(QString str);
    void SetFilterByListId(QStringList &list);
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    void SetOrderColum(int col);
    void showAll();
};

#endif // DATAMODEL_H
