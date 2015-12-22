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
    explicit dataModel(QObject *parent);

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    void refresh();
    int GetContactId(int row);
    Contact GetContact(int id);
    void deleteContact(int row);
    void setSeachString(QString str);

    void showAll();
};

#endif // DATAMODEL_H
