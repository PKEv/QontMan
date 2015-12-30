#ifndef TREEMODEL_H
#define TREEMODEL_H

#include <QAbstractItemModel>
#include <QVector>
#include "nodeinfo.h"

class TreeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit TreeModel(QObject *parent = 0);
    ~TreeModel();

    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;

    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);

    bool canFetchMore(const QModelIndex &parent) const;
    void fetchMore(const QModelIndex &parent);

    bool hasChildren(const QModelIndex &parent) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    QStringList getDescListId(const QModelIndex &parent);

private:
    typedef QVector<NodeInfo> NodeInfoList;
    NodeInfoList _nodes;
    Contact *head;

    void fetchRootDirectory();
    int findRow(const NodeInfo* nodeInfo) const;
    QVariant nameData(Contact& fileInfo, int role);
    void getListId(QStringList &list, const Contact &upLevel); // рекурсивная
};

#endif // TREEMODEL_H
