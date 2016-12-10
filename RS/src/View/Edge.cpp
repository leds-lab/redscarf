#include "include/View/Edge.h"
#include "include/View/Vertice.h"

#include <QPainter>

#ifndef M_PI
    #define M_PI       3.14159265358979323846
#endif

#include <cmath>

Edge::Edge(Vertice *source, Vertice *destination, QColor color) {

    this->source = source;
    this->destination = destination;
    this->color = color;
}

void Edge::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {

    painter->setPen(color);

    QPointF pontoOrigem = this->source->pos();
    QPointF pontoDestino = this->destination->pos();

    QLineF linha(pontoOrigem,pontoDestino);

    qreal anguloGraus = linha.angle();
    double seno = sin(anguloGraus * M_PI/180);
    double cosseno = cos(anguloGraus * M_PI/180);
    double catetoOposto = (Vertice::SIZE/2) * seno;
    double catetoAdjacente = (Vertice::SIZE/2)* cosseno;

    pontoDestino.setY(pontoDestino.y()+catetoOposto);
    pontoDestino.setX(pontoDestino.x()-catetoAdjacente);

    pontoOrigem.setY(pontoOrigem.y()-catetoOposto);
    pontoOrigem.setX(pontoOrigem.x()+catetoAdjacente);

    linha.setPoints(pontoOrigem,pontoDestino);

    // Desenha a linha de um vértice a outro
    painter->drawLine(linha);
}
