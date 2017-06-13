#include "Model/System/Topologies/Mesh3D.h"
#include "Model/System/Defines.h"

Mesh3D::Mesh3D(int xSize, int ySize, int zSize) : Topology(Topology::Mesh3D)
{
    adjacencyMatrix = QVector<QVector<int> >(xSize*ySize*zSize,QVector<int>(xSize*ySize*zSize,0));

    this->sp.setXSize(xSize);
    this->sp.setYSize(ySize);
    this->sp.setZSize(zSize);

    //Defining adjacency matrix
    for(int x = 0; x < xSize; x++){
        for(int y = 0; y < ySize; y++){
            for(int z = 0; z < zSize; z++){
                int address = COORDINATE_3D_TO_ID(x,y,z,xSize,ySize);

                bool useNorth = ( y < ySize-1 ) ? true : false;   // North port is not used on the top    border
                bool useEast  = ( x < xSize-1 ) ? true : false;   // East  port is not used on the right  border
                bool useSouth = ( y > 0 ) ? true : false;         // South port is not used on the bottom border
                bool useWest  = ( x > 0 ) ? true : false;         // West  port is not used on the left   border
                bool useUp    = ( z < zSize-1 ) ? true : false;   // Up    port is not used on the higher border
                bool useDown  = ( z < 0 ) ? true : false;         // Down  port is not used on the lower  border

                if(useNorth)
                    adjacencyMatrix[address][COORDINATE_3D_TO_ID(x,y+1,z,xSize,ySize)] = 1;
                if(useEast)
                    adjacencyMatrix[address][COORDINATE_3D_TO_ID(x+1,y,z,xSize,ySize)] = 1;
                if(useSouth)
                    adjacencyMatrix[address][COORDINATE_3D_TO_ID(x,y-1,z,xSize,ySize)] = 1;
                if(useWest)
                    adjacencyMatrix[address][COORDINATE_3D_TO_ID(x-1,y,z,xSize,ySize)] = 1;
                if(useUp)
                    adjacencyMatrix[address][COORDINATE_3D_TO_ID(x,y,z+1,xSize,ySize)] = 1;
                if(useDown)
                    adjacencyMatrix[address][COORDINATE_3D_TO_ID(x,y,z-1,xSize,ySize)] = 1;
            }
        }
    }
}
