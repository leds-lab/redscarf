#include "Model/System/Topologies/Switch.h"
#include "Model/System/Defines.h"

Switch::Switch(int n) : Topology(Topology::SwitchBus)
{
    adjacencyMatrix = QVector<QVector<int> >(n,QVector<int>(n,1));

    this->sp.setNumberElements(n);

    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            if(i != j)
                adjacencyMatrix[i][j] = 1;
        }
    }
}
