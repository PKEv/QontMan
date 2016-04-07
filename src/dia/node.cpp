/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "edge.h"
#include "node.h"
#include "graphwidget.h"

#include "../mainwindow.h"


#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>

Node::Node(GraphWidget *graphWidget)
    : graph(graphWidget)
{
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setZValue(-1);
}

void Node::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    int id = nodeInfo->cont.getId();
    Contact tcont = myModel->GetContact(id);
    passView = new passport(&tcont);
    passView->show();
}

void Node::addEdge(Edge *edge)
{
    edgeList << edge;
    edge->adjust();
}

QList<Edge *> Node::edges() const
{
    return edgeList;
}

bool Node::advance()
{
    if ((newPos == pos()) || (newPos==QPointF(0,0)))
        return false;

    setPos(newPos);
    return true;
}

QRectF Node::boundingRect() const
{

    QFont font("Arial", 18);
    font.setBold(true);
    QFontMetrics fm(font);

    QRectF rec = fm.boundingRect(nodeInfo->cont.getFullName());
    return rec;
    /*
    qreal adjust = 2;
    return QRectF( -10 - adjust, -10 - adjust, 23 + adjust, 23 + adjust);*/
}
/*
QPainterPath Node::shape() const
{
    QPainterPath path;
    path.addEllipse(-10, -10, 20, 20);
    return path;
}*/

void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    /* тень
    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::darkGray);
    painter->drawEllipse(-7, -7, 20, 20);
    */
    /*
    QRadialGradient gradient(-3, -3, 10);
    if (option->state & QStyle::State_Sunken) {
        gradient.setCenter(3, 3);
        gradient.setFocalPoint(3, 3);
        gradient.setColorAt(1, QColor(Qt::yellow).light(120));
        gradient.setColorAt(0, QColor(Qt::darkYellow).light(120));
    } else {
        gradient.setColorAt(0, Qt::yellow);
        gradient.setColorAt(1, Qt::darkYellow);
    }

    painter->setBrush(gradient);
    */
    //QFont font=painter->font() ;
    //font.setPointSize ( 18 );
    //painter->setFont(font);
    painter->setPen(QPen(Qt::black));
    //painter->drawRoundRect(QRect(-10, -10, 20, 20), 3, 3);
    //painter->drawText(QRect(0,0,200,500),nodeInfo->cont.getFullName());
    //painter->drawText(0,0, nodeInfo->cont.getFullName());

    QFont font("Arial", 18);
    font.setBold(true);
    painter->setFont(font);


    QFontMetrics fm(font);
    QRectF rec = fm.boundingRect(nodeInfo->cont.getFullName());

   // QRectF rec = painter->boundingRect(QRect(),nodeInfo->cont.getFullName(),QTextOption().alignment());
/*    QTextOption textOption(Qt::AlignCenter);
    textOption.setFlags(QTextOption::IncludeTrailingSpaces);
    painter->drawText(rec,QString(nodeInfo->cont.getFullName()), textOption);*/
    if (option->state & QStyle::State_Sunken)
    {
         painter->fillRect(rec,Qt::lightGray);
    }
    else
    {
        if (nodeInfo->cont.getTip()==1)
            painter->fillRect(rec,Qt::green);
        else
            painter->fillRect(rec,Qt::darkGreen);
    }

    painter->drawText(rec, Qt::AlignLeft, QString(nodeInfo->cont.getFullName()));
    painter->drawRoundedRect(rec,2,2);

    //painter->drawEllipse(-10, -10, 20, 20);
}

QVariant Node::itemChange(GraphicsItemChange change, const QVariant &value)
{

    switch (change) {
    case ItemPositionHasChanged:
        foreach (Edge *edge, edgeList)
            edge->adjust();
        break;
    default:
        break;
    };

    return QGraphicsItem::itemChange(change, value);
}

void Node::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mousePressEvent(event);
}

void Node::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}

