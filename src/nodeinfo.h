#ifndef NODEINFO_H
#define NODEINFO_H

#include <QVector>
#include "contact.h"

class NodeInfo
{
public:
    NodeInfo():parent(0), mapped(false)
    {};

    NodeInfo(const Contact & cont, NodeInfo * parent=0):
        cont(cont), parent(parent), mapped(false)
    {};

    //~NodeInfo();

    bool operator ==(const NodeInfo& another) const
    {
        bool r = this->cont == another.cont;
        Q_ASSERT(!r || this->parent == another.parent);
        Q_ASSERT(!r || this->mapped == another.mapped);
        Q_ASSERT(!r || this->children == another.children);
        return r;
    }

    Contact cont;
    NodeInfo *parent = nullptr;
    QVector<NodeInfo> children;
    bool mapped;
    struct
    {
        qreal min_x = std::numeric_limits<qreal>::max();
        qreal max_x = 0;
        qreal min_y = std::numeric_limits<qreal>::max();
        qreal max_y = 0;

    }graf ;
};

#endif // NODEINFO_H
