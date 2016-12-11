#include "View/Arc.h"
#include "View/Vertice.h"

#include <QPainter>

#ifndef M_PI
    #define M_PI       3.14159265358979323846
#endif

#include <cmath>

Arc::Arc(Vertice *source, Vertice *destination, int weight, QPen pen) {

    this->source = source;
    this->destination = destination;
    this->weight = weight;

    this->pen = pen;
}

void Arc::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {

    painter->setRenderHint(QPainter::Antialiasing,true);

    // Get points
    QPointF srcPoint = this->source->pos();
    QPointF destPoint = this->destination->pos();

    // Line between the points
    QLineF mainLine(srcPoint,destPoint);

    // Angulos da linha entre os ponto
    qreal angleDegree = mainLine.angle();
    double seno = sin(angleDegree * M_PI/180);
    double cosseno = cos(angleDegree * M_PI/180);
    double catetoOposto = (Vertice::SIZE/2) * seno;
    double catetoAdjacente = (Vertice::SIZE/2)* cosseno;

    // X and Y offset on destination
    destPoint.setY(destPoint.y()+catetoOposto);
    destPoint.setX((destPoint.x())-catetoAdjacente);

    // X and Y offset on source
    srcPoint.setY(srcPoint.y()-catetoOposto);
    srcPoint.setX((srcPoint.x())+catetoAdjacente);

    // Arch points
    QPointF point1 = srcPoint;
    QPointF point2 = destPoint;

    // Line base to arch draw
    QLineF line(point1,point2);

    line.setLength( line.length() / 2 );

    line.setAngle( line.angle() + anguloLinha(point1, point2));

    QPointF point3(line.x2(),line.y2());

    // Path from the point1
    QPainterPath testeArc(point1);
    testeArc.cubicTo(point3,point2,point2);

    //
    painter->setBrush(Qt::transparent);
    painter->setPen(pen);

    // Draw arch
    painter->drawPath(testeArc);

    this->setPath(testeArc);

    // Criação da ponta da seta
    QLineF line2(point3, destPoint);

    qreal up = line2.angle() + anguloLinha(destination->pos(), point3);
    qreal down = line2.angle() - anguloLinha(destination->pos(), point3);

    seno = sin(up*M_PI/180);
    cosseno = cos(up*M_PI/180);

    catetoOposto = (Vertice::SIZE/4) * seno;
    catetoAdjacente = (Vertice::SIZE/4) * cosseno;

    QPointF p1(destPoint.x()-catetoAdjacente,destPoint.y()+catetoOposto);

    seno = sin(down*M_PI/180);
    cosseno = cos(down*M_PI/180);

    catetoOposto = (Vertice::SIZE/4) * seno;
    catetoAdjacente = (Vertice::SIZE/4) * cosseno;

    QPointF p2(destPoint.x()-catetoAdjacente,destPoint.y()+catetoOposto);

    QPainterPath pathSeta(destPoint);

    // Criação dos caminhos da seta
    pathSeta.lineTo(p1);
    pathSeta.lineTo(p2);
    pathSeta.lineTo(destPoint);

    // Modifica a cor do pincel
    painter->setBrush(Qt::black);
    painter->setPen(Qt::black);
    // Desenha o caminho da seta
    painter->drawPath(pathSeta);
}

int Arc::verificaQuadrante(QPointF ponto){

    if(ponto.x() > 0) {
        if(ponto.y() > 0) {
            return 1;
        } else if (ponto.y() < 0) {
            return 4;
        } else {
            return 1;
        }
    } else {
        if(ponto.y() > 0) {
            return 2;
        } else if(ponto.y() < 0){
            return 3;
        } else {
            return 2;
        }
    }
}

int Arc::anguloLinha(QPointF origem, QPointF destino) {

    int quadranteOrigem = verificaQuadrante(origem);
    int quadranteDestino = verificaQuadrante(destino);

    switch(quadranteOrigem) {

        case 1: {
            if(quadranteDestino > 0 && quadranteDestino < 5) {
                int temp = direcao(origem, destino);
                return temp;
            }
            break;
        }
        case 2: {
            if(quadranteDestino > 0 && quadranteDestino < 5) {
                int temp = direcao(origem, destino);
                return temp;
            }
            break;
        }
        case 3: {
            if(quadranteDestino > 0 && quadranteDestino < 5) {
                int temp = direcao(origem, destino);
                return temp;
            }
            break;
        }
        case 4: {
            if(quadranteDestino > 0 && quadranteDestino < 5) {
                int temp = direcao(origem, destino);
                return temp;
            }
            break;
        }
    }
    return 0;
}

int Arc::direcao(QPointF origem, QPointF destino) {

    int diferencaX = destino.x() - origem.x();
    int diferencaY = destino.y() - origem.y();

    // Deslocamento para a direita
    if(diferencaX > 0) {
        // Diagonal superior direita
        if(diferencaY > 0) {
            return -30;
        }
        // Diagonal inferior direita
        else if (diferencaY < 0) {
            return 30;
        }
        // Direita
        else {
            return -30;
        }
    }
    // Deslocamento para a esquerda
    else if(diferencaX < 0) {
        // Diagonal superior esquerda
        if(diferencaY > 0) {
            return 30;
        }
        // Diagonal inferior esquerda
        else if (diferencaY < 0) {
            return -30;
        }
        // Esquerda
        else {
            return -30;
        }
    }
    // Deslocamento p/ cima ou p/ baixo
    else {
        // Cima
        if(diferencaY > 0) {
            return -30;
        }
        // Baixo
        else {
            return -30;
        }
    }
    return 0;
}
