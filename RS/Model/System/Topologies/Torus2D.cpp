#include "Model/System/Topologies/Torus2D.h"
#include "Model/System/Defines.h"

Torus2D::Torus2D(int x, int y) : Topology(Topology::Mesh2D)
{
    adjacencyMatrix = QVector<QVector<int> >(x,QVector<int>(y,0));

    this->sp.setXSize(x);
    this->sp.setYSize(y);

    //Defining adjacency matrix
    for(int i = 0; i < x; i++){
        for(int j = 0; j < y; j++){
            int address = COORDINATE_2D_TO_ID(i,j,x);
            adjacencyMatrix[address][COORDINATE_2D_TO_ID(i,j+1,x)] = 1;
            adjacencyMatrix[address][COORDINATE_2D_TO_ID(i+1,j,x)] = 1;
            adjacencyMatrix[address][COORDINATE_2D_TO_ID(i,j-1,x)] = 1;
            adjacencyMatrix[address][COORDINATE_2D_TO_ID(i-1,j,x)] = 1;
        }
    }
}
