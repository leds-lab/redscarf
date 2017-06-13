#ifndef __TOPOLOGY_H__
#define __TOPOLOGY_H__

#include "Model/System/SystemParameters.h"
#include <QVector>

class Topology {

public:
    enum ETopology{ Mesh2D = 1,
                    SwitchBus,
                    SwitchCrossbar = SwitchBus,
                    Ring,
                    ChordalRing,
                    Torus2D,
                    Mesh3D
    };
protected:
    QVector<QVector<int> > adjacencyMatrix;
    ETopology topology;
public:
    SystemParameters sp;

    static ETopology indexToEnum(unsigned int index) {
        if( index < 3 ) {
            return static_cast<ETopology>(index+1);
        }
        return static_cast<ETopology>(index);
    }

    void readLogs();

    Topology(ETopology topology){
        this->topology = topology;
        switch(topology){
            case Mesh2D:
            case Torus2D:
                this->sp.setTopologyType(SystemParameters::Orthogonal2D);
                break;
            case Mesh3D:
                this->sp.setTopologyType(SystemParameters::Orthogonal3D);
                break;
            default:
                this->sp.setTopologyType(SystemParameters::NonOrthogonal);
        }
    }

//    void drawTopology(Cena, Vertices<Nodos>, Arcos<Fluxos>);

    ETopology getTopology(){
        return topology;
    }
    SystemParameters::TopologyType getTopologyType(){
        return sp.getTopologyType();
    }
    QVector<QVector<int> > getAdjacencyMatrix(){
        return adjacencyMatrix;
    }
};

#endif // __TOPOLOGY_H__
