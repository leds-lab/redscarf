#ifndef __VERTICE_H__
#define __VERTICE_H__

#include <QGraphicsItem>

class Vertice : public QGraphicsItem {
    Q_INTERFACES(QGraphicsItem)
private:
    int id;
    QColor color;
    QColor selected;
public:
    static const int SIZE = 40;
    explicit Vertice(int id,QColor color = Qt::lightGray);

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void setColor(QColor color);
    QColor getColor();

    inline void setId(int id) { this->id = id; }
    inline int getId() const { return this->id; }
};

#endif // __VERTICE_H__
