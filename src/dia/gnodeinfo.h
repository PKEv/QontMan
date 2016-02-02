#ifndef GNODEINFO_H
#define GNODEINFO_H

#include "../nodeinfo.h"
#include "node.h"


class GNodeInfo : public NodeInfo
{
public:
    GNodeInfo():NodeInfo()
    {};

    GNodeInfo(const Contact & cont, NodeInfo * parent=0) : NodeInfo(cont, parent)
    {
        g_node = nullptr;
    };

    Node * g_node;
    ~GNodeInfo();
};

#endif // GNODEINFO_H
