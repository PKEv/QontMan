#ifndef GNODEINFO_H
#define GNODEINFO_H

#include "../nodeinfo.h"
#include "node.h"


class GNodeInfo : public NodeInfo
{
public:
    GNodeInfo():NodeInfo()
    {
        group=0;
        level=0;
    };

    GNodeInfo(const Contact & cont, NodeInfo * parent=0) : NodeInfo(cont, parent)
    {
       g_node = nullptr;
       group=0;
       level=0;
    };

    ~GNodeInfo();

    Node * g_node;
    int group;
    int level;
};

#endif // GNODEINFO_H
