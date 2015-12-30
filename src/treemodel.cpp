#include "treemodel.h"
#include "PlainDb.h"

TreeModel::TreeModel(QObject *parent) : QAbstractItemModel(parent)
{
    fetchRootDirectory();
}

TreeModel::~TreeModel()
{
 delete head;
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
    {
        return QModelIndex();
    }

    if (!parent.isValid())
    {
        Q_ASSERT(_nodes.size() > row);
        return createIndex(row, column, const_cast<NodeInfo*>(&_nodes[row]));
    }

    NodeInfo* parentInfo = static_cast<NodeInfo*>(parent.internalPointer());
    Q_ASSERT(parentInfo != 0);
    Q_ASSERT(parentInfo->mapped);
    Q_ASSERT(parentInfo->children.size() > row);

    return createIndex(row, column, &parentInfo->children[row]);
}

QModelIndex TreeModel::parent(const QModelIndex &child) const
{
    if (!child.isValid()) {
        return QModelIndex();
    }

    NodeInfo* childInfo = static_cast<NodeInfo*>(child.internalPointer());
    Q_ASSERT(childInfo != 0);
    NodeInfo* parentInfo = childInfo->parent;
    if (parentInfo != 0) {
        return createIndex(findRow(parentInfo), 1, parentInfo);
    }
    else {
        return QModelIndex();
    }
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
    Q_UNUSED(parent);
    return 1;
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    NodeInfo* nodeInfo = static_cast<NodeInfo*>(index.internalPointer());
    QString name = nodeInfo->cont.getFullName();
    Q_ASSERT(nodeInfo != 0);
    if (role == Qt::DisplayRole)
        return name;
    return QVariant();
}

bool TreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    Q_UNUSED(index);
    Q_UNUSED(value);
    Q_UNUSED(role);
    return false;
}

bool TreeModel::hasChildren(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        const NodeInfo* parentInfo = static_cast<const NodeInfo*>(parent.internalPointer());
        Q_ASSERT(parentInfo != 0);
        if (!parentInfo->mapped)
        {
            std::vector<Contact> vec;
            PlainDb::getInstance()->GetContactsListByUplevel(vec, parentInfo->cont.getId());
            if (vec.size() > 0)
                return true;
        }        
    }
    return QAbstractItemModel::hasChildren(parent);

}

void TreeModel::fetchRootDirectory()
{

    _nodes.clear();
    head = new Contact();
    head->setId(0);
    head->setFullName(tr("Все"));
    //head->parent = nullptr;
    _nodes.push_back(NodeInfo(*head));
//*/
/*
    std::vector<Contact> vec;
    PlainDb::getInstance()->GetContactsListByUplevel(vec, 0);
    for(unsigned int i=0; i < vec.size() ; i++)
    {
        _nodes.push_back(NodeInfo(vec.at(i)));
    }
  //  */
}

int TreeModel::findRow(const NodeInfo* nodeInfo) const
{
    Q_ASSERT(nodeInfo != 0);
    const NodeInfoList& parentInfoChildren = nodeInfo->parent != 0 ? nodeInfo->parent->children: _nodes;
    if (nodeInfo->cont.getId() == head->getId()) // особенности функции сравнения
    {
        return 0; // корневой узел у нас точно нулевой, мы его сами сделали в функции fetchRootDirectory
    }
    NodeInfoList::const_iterator position = qFind(parentInfoChildren, *nodeInfo);
    Q_ASSERT(position != parentInfoChildren.end());
    return std::distance(parentInfoChildren.begin(), position);
}

bool TreeModel::canFetchMore(const QModelIndex &parent) const
{
    if (!parent.isValid()) {
        return false;
    }

    const NodeInfo* parentInfo = static_cast<const NodeInfo*>(parent.internalPointer());
    Q_ASSERT(parentInfo != 0);
    return !parentInfo->mapped;
}
void TreeModel::fetchMore(const QModelIndex &parent)
{
    Q_ASSERT(parent.isValid());
    NodeInfo* parentInfo = static_cast<NodeInfo*>(parent.internalPointer());
    Q_ASSERT(parentInfo != 0);
    Q_ASSERT(!parentInfo->mapped);

    std::vector<Contact> children;
    PlainDb::getInstance()->GetContactsListByUplevel(children, parentInfo->cont.getId());

    int insrtCnt = children.size();

    beginInsertRows(parent, 0, insrtCnt);
    for (const Contact& entry: children) {
        NodeInfo nodeInfo(entry, parentInfo);
        nodeInfo.mapped = false;
        parentInfo->children.push_back(std::move(nodeInfo));
    }
    parentInfo->mapped = true;
    endInsertRows();
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    const QStringList headers = {"Имя"};
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole && section < headers.size()) {
        return headers[section];
    }
    return QVariant();
}
