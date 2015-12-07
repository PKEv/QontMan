#ifndef DATAMODEL_H
#define DATAMODEL_H

#include "PlainDb.h"
#include <QSqlQueryModel>
#include <QString>

class dataModel : public QSqlQueryModel
{
    Q_OBJECT
public:
    explicit dataModel(QObject *parent);

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    void refresh();

    void showAll();
};

#endif // DATAMODEL_H
