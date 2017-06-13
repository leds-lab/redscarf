#ifndef __MESH3D_H__
#define __MESH3D_H__

#include "Model/System/Topology.h"

class Mesh3D : public Topology {
public:
    Mesh3D(int xSize, int ySize, int zSize);
};

#endif // __MESH3D_H__
