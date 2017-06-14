#include "Model/System/Topologies/Mesh2D.h"
#include "Model/System/Defines.h"
#include <QDebug>

Mesh2D::Mesh2D(int xSize, int ySize) : Topology(Topology::Mesh2D)
{
    adjacencyMatrix = QVector<QVector<int> >(xSize*ySize,QVector<int>(xSize*ySize,0));

    this->sp.setXSize(xSize);
    this->sp.setYSize(ySize);

    //Defining adjacency matrix
    for(int x = 0; x < xSize; x++){
        for(int y = 0; y < ySize; y++){
            int address = COORDINATE_2D_TO_ID(x,y,xSize);

            bool useNorth = ( y < ySize-1 ) ? true : false;   // North port is not used on the top    border
            bool useEast  = ( x < xSize-1 ) ? true : false;   // East  port is not used on the right  border
            bool useSouth = ( y > 0 ) ? true : false;         // South port is not used on the bottom border
            bool useWest  = ( x > 0 ) ? true : false;         // West  port is not used on the left   border

            if(useNorth)
                adjacencyMatrix[address][COORDINATE_2D_TO_ID(x,y+1,xSize)] = 1;
            if(useEast)
                adjacencyMatrix[address][COORDINATE_2D_TO_ID(x+1,y,xSize)] = 1;
            if(useSouth)
                adjacencyMatrix[address][COORDINATE_2D_TO_ID(x,y-1,xSize)] = 1;
            if(useWest)
                adjacencyMatrix[address][COORDINATE_2D_TO_ID(x-1,y,xSize)] = 1;
        }
    }
}
