#ifndef STRUCTMODEL_H
#define STRUCTMODEL_H

#include <QAbstractItemModel>
#include <QVector>

class StructModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit StructModel(QObject *parent = 0);
    ~StructModel();
};

#endif // STRUCTMODEL_H
