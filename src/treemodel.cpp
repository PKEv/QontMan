#include "treemodel.h"
#include "PlainDb.h"

TreeModel::TreeModel(QObject *parent) : QAbstractItemModel(parent)
{
    fetchRootDirectory();
}

TreeModel::~TreeModel()
{

}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
{
    /*
    if (!hasIndex(row, column, parent))
    {
        return QModelIndex();
    }

    if (!parent.isValid())
    {
        Q_ASSERT(_nodes.size() > row);
        return createIndex(row, column, const_cast<NodeInfo*>(&_nodes[row]));
    }*/

     return QModelIndex();

}

QModelIndex TreeModel::parent(const QModelIndex &child) const
{

}

int TreeModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid()) {
        return _nodes.size();
    }
    const NodeInfo* parentInfo = static_cast<const NodeInfo*>(parent.internalPointer());
    Q_ASSERT(parentInfo != 0);

    return parentInfo->children.size();
}

int TreeModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 1;
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{

}

bool TreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{

}

bool TreeModel::hasChildren(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        const NodeInfo* parentInfo = static_cast<const NodeInfo*>(parent.internalPointer());
        Q_ASSERT(parentInfo != 0);
        if (!parentInfo->mapped) {
            return true;//QDir(parentInfo->fileInfo.absoluteFilePath()).count() > 0;
        }
    }
    return QAbstractItemModel::hasChildren(parent);

}

void TreeModel::fetchRootDirectory()
{
    _nodes.clear();
    std::vector<Contact> vec;
    PlainDb::getInstance()->GetContactsListByUplevel(vec, 0);
    for(int i=0; i < vec.size() ; i++)
    {
        _nodes.push_back(NodeInfo(vec.at(i)));
    }

}

int TreeModel::findRow(const NodeInfo* nodeInfo) const
{

}
