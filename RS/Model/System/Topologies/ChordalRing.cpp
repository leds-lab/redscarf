#include "Model/System/Topologies/ChordalRing.h"
#include "Model/System/Defines.h"

ChordalRing::ChordalRing(int n) : Topology(Topology::ChordalRing)
{
    adjacencyMatrix = QVector<QVector<int> >(n,QVector<int>(n,0));
    this->sp.setNumberElements(n);

    //Defining adjacency matrix
    for(int i = 0; i < n-1; i++){
        adjacencyMatrix[i][i+1] = 1;
        adjacencyMatrix[i+1][i] = 1;
        adjacencyMatrix[i][i+(n/2)] = 1;
        adjacencyMatrix[i+(n/2)][i] = 1;
    }
    adjacencyMatrix[n-1][0] = 1;
    adjacencyMatrix[0][n-1] = 1;
    adjacencyMatrix[n-1][(n/2)-1] = 1;
    adjacencyMatrix[(n/2)-1][n-1] = 1;
}
