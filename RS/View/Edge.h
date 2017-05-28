#ifndef __EDGE_H__
#define __EDGE_H__

#include <QGraphicsItem>

class Vertice;

class Edge : public QGraphicsPathItem {
private:

    Vertice* source;
    Vertice* destination;
    QColor color;

public:
    Edge(Vertice* source,Vertice* destination,QColor color = Qt::black);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

};

#endif // __EDGE_H__
