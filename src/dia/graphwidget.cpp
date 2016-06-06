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

#include "graphwidget.h"
#include "edge.h"
#include "../PlainDb.h"
#include <math.h>
#include <QDebug>

#include <QKeyEvent>

//! [0]
GraphWidget::GraphWidget(QWidget *parent)
    : QGraphicsView(parent)
{
    /*
    this->id = 0;
    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
   // scene->setSceneRect(-200, -200, 400, 400);
    setScene(scene);
    setCacheMode(CacheBackground);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    //scale(qreal(2), qreal(2));
    setMinimumSize(400, 400);
    setDragMode(QGraphicsView::ScrollHandDrag);

    fillNodes();
    optimizNodePos();

    // отображаем собранную информацию по узлам
    Node *node = nullptr;
    foreach (node, nodes)
    {
        scene->addItem(node);
        node->setPos(node->y * node->boundingRect().height() * 2,
                     node->x * node->boundingRect().width() * 2);
    }

    Edge *edge;
    foreach (edge, edges)
    {
        scene->addItem(edge);
    }
    // Размер сцены
    if (node != nullptr)
    {
        int w =  fieldSize.x * node->boundingRect().width() * 2;
        int h =  fieldSize.y * node->boundingRect().width() * 2;
        //viewport()->geometry()
        //setSceneRect(-h, -w, h*3, w*3);
        //scale(0.2, 0.2);
        centerOn(h/2,w/2);
        fitInView(-h, -w, h*3, w*3,Qt::KeepAspectRatio);
    }
    setAlignment(Qt::AlignLeft | Qt::AlignHCenter);
    */
}

GraphWidget::GraphWidget(int id, QWidget *parent)
    : QGraphicsView(parent)
{
    this->id = id;
    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
   // scene->setSceneRect(-200, -200, 400, 400);
    setScene(scene);
    setCacheMode(CacheBackground);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    //scale(qreal(2), qreal(2));
    setMinimumSize(400, 400);
    setDragMode(QGraphicsView::ScrollHandDrag);

    fillNodes();
    optimizNodePos();

    // отображаем собранную информацию по узлам
    Node *node = nullptr;
    foreach (node, nodes)
    {
        scene->addItem(node);
        node->setPos(node->y * node->boundingRect().height() * 2,
                     node->x * node->boundingRect().width() * 2);
    }

    Edge *edge;
    foreach (edge, edges)
    {
        scene->addItem(edge);
    }
    // Размер сцены
    if (node != nullptr)
    {
        int w =  fieldSize.x * node->boundingRect().width() * 2;
        int h =  fieldSize.y * node->boundingRect().width() * 2;
        //viewport()->geometry()
        //setSceneRect(-h, -w, h*3, w*3);
        //scale(0.2, 0.2);
        centerOn(h/2,w/2);
        fitInView(-h, -w, h*3, w*3,Qt::KeepAspectRatio);
    }
    setAlignment(Qt::AlignLeft | Qt::AlignHCenter);
}

GraphWidget::~GraphWidget()
{
    qDeleteAll(nodes.begin(), nodes.end());
    qDeleteAll(edges.begin(), edges.end());
    qDeleteAll(nodesInfo.begin(), nodesInfo.end());
    nodes.clear();
    edges.clear();
    nodesInfo.clear();
}

/*
void GraphWidget::mouseMoveEvent(QMouseEvent * event)
{
    if(!this->fixedPoint.isNull())
    {
            QPointF offset = this->fixedPoint - pos;             // вычисляем разность, на которую мышь подвинулась, между начальной точкой и конечной

            centerOn(center-offset);
            //this->setCenter(this->getCenter() - offset);       // находим новую точку-центр, в которую надо передвинуть сцену, и передаем

            this->scene()->update();
    }
}
*/


void GraphWidget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Plus:
        zoomIn();
        break;
    case Qt::Key_Minus:
        zoomOut();
        break;
    case Qt::Key_Space:
    default:
        QGraphicsView::keyPressEvent(event);
    }

}

#ifndef QT_NO_WHEELEVENT
void GraphWidget::wheelEvent(QWheelEvent *event)
{
   scaleView(pow((double)2, -event->delta() / 240.0));
   /*
    // Scale the view / do the zoom
        const double scaleFactor = 1.15;
        if(event->delta() > 0)
        {
            // Zoom in
            scale(scaleFactor, scaleFactor);
        }
        else
        {
            // Zooming out
            scale(1.0 / scaleFactor, 1.0 / scaleFactor);
        }
*/
   this->scene()->update();
}
#endif


void GraphWidget::drawBackground(QPainter *painter, const QRectF &rect)
{
    Q_UNUSED(rect);
    Q_UNUSED(painter);

    // Shadow
/*
    QRectF sceneRect = this->sceneRect();

    QRectF rightShadow(sceneRect.right(), sceneRect.top() + 5, 5, sceneRect.height());
    QRectF bottomShadow(sceneRect.left() + 5, sceneRect.bottom(), sceneRect.width(), 5);
    if (rightShadow.intersects(rect) || rightShadow.contains(rect))
        painter->fillRect(rightShadow, Qt::darkGray);
    if (bottomShadow.intersects(rect) || bottomShadow.contains(rect))
        painter->fillRect(bottomShadow, Qt::darkGray);
    */
    // Fill
/*
    QLinearGradient gradient(rect.topLeft(), rect.bottomRight());
    gradient.setColorAt(0, Qt::white);
    gradient.setColorAt(1, Qt::lightGray);
    painter->fillRect(rect.intersected(rect), gradient);
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(rect);
    this->scene()->update();
    //*/
    // Text
    /*
    QRectF textRect(sceneRect.left() + 4, sceneRect.top() + 4,
                    sceneRect.width() - 4, sceneRect.height() - 4);
    QString message(tr("Click and drag the nodes around, and zoom with the mouse "
                       "wheel or the '+' and '-' keys"));

    QFont font = painter->font();
    font.setBold(true);
    font.setPointSize(14);
    painter->setFont(font);
    painter->setPen(Qt::lightGray);
    painter->drawText(textRect.translated(2, 2), message);
    painter->setPen(Qt::black);
    painter->drawText(textRect, message);
    */
}

void GraphWidget::scaleView(qreal scaleFactor)
{
    qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.01 || factor > 100)
        return;

    scale(scaleFactor, scaleFactor);
}

void GraphWidget::zoomIn()
{
    scaleView(qreal(2));
}

void GraphWidget::zoomOut()
{
    scaleView(1 / qreal(2));
}

void GraphWidget::fillNodes()
{
    // заполняем nodesInfo
    std::vector<Contact> vec;
    Contact cont;
    if (id == 0)
        PlainDb::getInstance()->GetContactsListByUplevel(vec, id);
    else
        vec.push_back(PlainDb::getInstance()->getContById(id));

    int x=0, y=0;
    foreach (cont, vec)
    {
        NodeInfo * node = new NodeInfo(cont, 0);
        Node * g_node = new Node(this);
        g_node->nodeInfo = node;
        g_node->x = x;
        g_node->y = y;
        recursivNodesInfo(node, g_node);
        nodesInfo.push_back(node);
        nodes.push_back(g_node);

        // подсчитываем размер поля для отображения
        if (fieldSize.x < x)
            fieldSize.x = x;

        y = fieldSize.y;
        y++;
    }
}

void GraphWidget::recursivNodesInfo(NodeInfo *parent, Node *parentNode )
{
    std::vector<Contact> vec;
    Contact cont;
    PlainDb::getInstance()->GetContactsListByUplevel(vec, parent->cont.getId());
    int y = parentNode->y;
    if (y < fieldSize.y)
        y = fieldSize.y;

    int x = parentNode->x +1;
    foreach (cont, vec)
    {
        NodeInfo * node = new NodeInfo(cont, parent);
        Node * g_node = new Node(this);
        g_node->nodeInfo = node;
        if (y < fieldSize.y)
            y = fieldSize.y;
        g_node->x = x;
        g_node->y = y;
        recursivNodesInfo(node, g_node);
        parent->children.push_back(*node);
        nodes.push_back(g_node);

        Edge *edge = new Edge(g_node, parentNode);
        edges.push_back(edge);

        // подсчитываем размер поля для отображения
        if (fieldSize.x < x)
            fieldSize.x = x;
        y++;
        if (fieldSize.y < y)
            fieldSize.y = y;
    }
}

void GraphWidget::optimizNodePos()
{
    int matrix [fieldSize.x+1][fieldSize.y+1];

    for ( int i = 0; i < fieldSize.x+1; ++i )
               for ( int j = 0; j < fieldSize.y+1; ++j )
                   matrix[ i ][ j ] = 0;

    Node *node;
    foreach (node, nodes)
    {
        matrix[int(node->x)][int(node->y)]=1;
    }

    for ( int i = 0; i < fieldSize.x+1; ++i )
        for ( int j = 0; j < fieldSize.y+1; ++j )
        {
            if (matrix[ i ][ j ] == 1)
            {
                int jj = j+1;
                for ( ; jj < fieldSize.y+1; ++jj )
                {
                    if (matrix[ i ][ jj ] == 1)
                        break;
                }
                //исключаем движение крайних элементов и лишние вычисление при фактическом отсутствии движения
                if (! ( (jj == j + 1) || (jj == fieldSize.y+1) ) )
                {
                    Node* node = findNodeByPos(i,j);
                    node->y = ((jj-j)/2+j);
                }
                /*
                if ( i>0 )
                {
                    Node* node = findNodeByPos(i,j);
                    if (j % 2==0)
                        node->x = (i+0.5);
                    else
                        node->x = (i-0.5);
                }
                  */
            }
        }
}

Node * GraphWidget::findNodeByPos(int x, int y)
{
    Node *node;
    foreach (node, nodes)
    {
        if (node->x == x && node->y == y)
            return node;
    }
    return nullptr;
}
