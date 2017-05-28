#ifndef __SYSTEM_PARAMETERS_H__
#define __SYSTEM_PARAMETERS_H__

#include <QApplication>

class SystemParameters {
    Q_DECLARE_TR_FUNCTIONS(SystemParameters)
public:
    enum TopologyType { NonOrthogonal, Orthogonal2D,Orthogonal3D };

    SystemParameters();
    ~SystemParameters();

    bool equal(SystemParameters&) const;

    // Set
    inline void setTopologyType(TopologyType type) { this->type = type; }
    inline void setNumberElements(unsigned short number) { this->numElements = number; }
    inline void setXSize(unsigned short size) { this->xSize = size; }
    inline void setYSize(unsigned short size) { this->ySize = size; }
    inline void setZSize(unsigned short size) { this->zSize = size; }
    inline void setDataWidth(unsigned short width) { this->dataWidth = width; }
    // Get
    static TopologyType getTopologyType(QString name);
    inline TopologyType getTopologyType() const { return this->type; }
    inline unsigned short getNumberElements() const {
        switch(type) {
            case NonOrthogonal:
                return numElements;
            case Orthogonal2D:
                return (xSize * ySize);
            case Orthogonal3D:
                return (xSize * ySize * zSize);
            default: break;
        }
        return this->numElements;
    }
    inline unsigned short getXSize() const { return this->xSize; }
    inline unsigned short getYSize() const { return this->ySize; }
    inline unsigned short getZSize() const { return this->zSize; }
    inline unsigned short getDataWidth() const { return this->dataWidth; }

    QString getFormattedString() const;

private:
    TopologyType type;
    unsigned short numElements;
    unsigned short xSize;
    unsigned short ySize;
    unsigned short zSize;

    unsigned short dataWidth;
};

Q_DECLARE_METATYPE( SystemParameters )

#endif // __SYSTEM_PARAMETERS_H__
