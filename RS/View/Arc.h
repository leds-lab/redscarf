#ifndef __ARC_H__
#define __ARC_H__

#include <QGraphicsItem>
#include <QPen>

class Vertice;

class Arc : public QGraphicsPathItem {
private:

    Vertice* source;
    Vertice* destination;
    int weight;
    QPen pen;

    /**
     * @brief verificaQuadrante: Verifica em qual quadrante o seu ponto está
     * @param ponto: Coordenada x,y no plano
     * @return Quadrante aonde está o ponto
     */
    int verificaQuadrante(QPointF ponto);

    /**
     * @brief anguloLinha: Verifica a ser somado ou diminuido da linha dependendo do quadrante aonde se encontram
     * @param origem: Coordenada x,y do ponto de origem
     * @param destino: Coordenada x,y do ponto de destino
     * @return Angulo
     */
    int anguloLinha(QPointF origem, QPointF destino);

    /**
     * @brief direcao: Derermina a direção e o sinal do angulo
     * @param origem: coordenada x,y do ponto de origem
     * @param destino: coordenada x,y do ponto de destino
     * @return Direção da ligação
     */
    int direcao(QPointF origem, QPointF destino);


public:
    Arc(Vertice* source, Vertice* destination, int weight, QPen pen = QPen());

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    inline Vertice* getSource() const { return this->source; }
    inline Vertice* getDestination() const { return this->destination; }

};

#endif // __ARC_H__
