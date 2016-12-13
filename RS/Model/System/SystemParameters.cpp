#include "Model/System/SystemParameters.h"

SystemParameters::SystemParameters(){
    type = NonOrthogonal;
    numElements = 0;
    xSize = 0;
    ySize = 0;
    zSize = 0;
    dataWidth = 0;
}

SystemParameters::~SystemParameters() {}

QString SystemParameters::getFormattedString() const {
    QString formatted;
    switch( type ) {
        case 0: // Non-Orthogonal
            formatted = tr("Non-Orthogonal - %1 elements")
                    .arg(numElements);
            break;
        case 1: // 2D-Orthogonal
            formatted = tr("2D-Orthogonal - %1 (%2x%3) elements")
                    .arg( (xSize*ySize) )
                    .arg(xSize)
                    .arg(ySize);
            break;
        case 2: // 3D-Orthogonal
            formatted = tr("3D-Orthogonal - %1 (%2x%3x%4) elements")
                    .arg( (xSize*ySize*zSize) )
                    .arg(xSize)
                    .arg(ySize)
                    .arg(zSize);
            break;
        default:break;
    }
    formatted += tr(" - Data Width: %1-bit").arg(dataWidth);
    return formatted;
}

bool SystemParameters::equal(SystemParameters &sp) const {
    if( type == sp.type ) {
        if( dataWidth == sp.dataWidth ) {
            switch( type ) {
                case 0: // Non-Orthogonal
                    if( numElements == sp.numElements ) {
                        return true;
                    }
                    break;
                case 1: // 2D-Orthogonal
                    if( xSize == sp.xSize && ySize == sp.ySize) {
                        return true;
                    }
                    break;
                case 2: // 3D-Orthogonal
                    if( xSize == sp.xSize && ySize == sp.ySize && zSize == sp.zSize) {
                        return true;
                    }
                    break;
                default:break;
            }

        }

    }
    return false;
}

SystemParameters::TopologyType SystemParameters::getTopologyType(QString name) {

    if( name.compare("NonOrthogonal") == 0 ) {
        return SystemParameters::NonOrthogonal;
    } else if(name.compare("Orthogonal2D") == 0) {
        return SystemParameters::Orthogonal2D;
    } else if(name.compare("Orthogonal3D") == 0) {
        return SystemParameters::Orthogonal3D;
    } else {
        return SystemParameters::NonOrthogonal; // Default value
    }
}
