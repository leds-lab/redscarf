#include "Model/Traffic/TrafficParameters.h"

TrafficParameters::TrafficParameters() {

    this->alfaOff                       = 1.90f;
    this->alfaOn                        = 1.25f;
    this->deadline                      = 0;
    this->destination                   = 0;
    this->idleTime                      = 0;
    this->injectionType                 = 0;
    this->intervalTime                  = 0;
    this->numberRates                   = 100;
    this->packageToSend                 = 0;
    this->messageSize                   = 1;
    this->payloadLength                 = 0;
    this->probabilityFunction           = 0;
    this->requiredBandwidth             = 0;
    this->requiredBandwidthStdDeviation = 0.01f;
    this->source                        = 0;
    this->distribution                  = SpatialDistribution::Specific_Address;
    this->switchingTechnique            = 0;
    this->trafficClass                  = 0;
}


/**
 * return A string containing the status of the attributes
 */
QString TrafficParameters::toString() const {

    // Assembling string
    QString formatted;

    formatted = tr("Spatial Distribution: %1\n"
                   "Source: %2\n"
                   "Destination: %3\n"
                   "Traffic Class: %4\n"
                   "Injection Type: %5\n"
                   "Switching Technique: %6\n")
            .arg(distribution)
            .arg(source)
            .arg(destination)
            .arg(trafficClass)
            .arg(injectionType)
            .arg(switchingTechnique);

    formatted += tr("Packages to Send: %1\n"
                    "Deadline: %2\n"
                    "Required BW: %3\n"
                    "Payload length: %4\n"
                    "Message size: %5\n"
                    "Idle cycles: %6\n")
            .arg(packageToSend)
            .arg(deadline)
            .arg(requiredBandwidth)
            .arg(payloadLength)
            .arg(messageSize)
            .arg(idleTime);

    formatted += tr("Interval Time: %1\n"
                     "Prob. function: %2\n"
                     "Req. BW Std. Deviation: %3\n"
                     "Alfa ON: %4\n"
                     "Alfa OFF: %5\n"
                     "Number rates: %6")
            .arg(intervalTime)
            .arg(probabilityFunction)
            .arg(requiredBandwidthStdDeviation)
            .arg(alfaOn)
            .arg(alfaOff)
            .arg(numberRates);

    return formatted;
}

/**
 * return true if the object parameter has the same values in its attributes, false otherwise or if it is null
 */

bool TrafficParameters::equals(TrafficParameters* obj) const {

    if(obj == NULL) {
        return false;
    }
    if(obj->source                  == this->source                &&
            obj->distribution       == this->distribution          &&
            obj->destination        == this->destination           &&
            obj->trafficClass       == this->trafficClass          &&
            obj->injectionType      == this->injectionType         &&
            obj->switchingTechnique == this->switchingTechnique    &&
            obj->packageToSend      == this->packageToSend         &&
            obj->deadline           == this->deadline              &&
            obj->requiredBandwidth  == this->requiredBandwidth     &&
            obj->payloadLength      == this->payloadLength         &&
            obj->messageSize        == this->messageSize           &&
            obj->idleTime           == this->idleTime              &&
            obj->intervalTime       == this->intervalTime          &&
            obj->probabilityFunction== this->probabilityFunction   &&
            obj->alfaOn             == this->alfaOn                &&
            obj->alfaOff            == this->alfaOff               &&
            obj->numberRates        == this->numberRates           &&
            obj->requiredBandwidthStdDeviation == this->requiredBandwidthStdDeviation
            ) {
        return true;
    }

    return false;
}

QString TrafficParameters::getFormattedString() const {

    QString formatted = this->getSpatialDistributionName();

    formatted += tr(" - from %1 to %2")
            .arg(source)
            .arg(destination);

    if( deadline > 0 ) {
        formatted += tr(" - Deadline: %1").arg(deadline);
    }

    formatted += tr(" - Req. BW(Mbps): %1").arg(requiredBandwidth);

    return formatted;
}

void TrafficParameters::setSpatialDistribution(int index) {
    SpatialDistribution::Distribution dist = static_cast<SpatialDistribution::Distribution>(index);
    this->setSpatialDistribution(dist);
}

QString TrafficParameters::getSpatialDistributionName(SpatialDistribution::Distribution dist) const {
    switch (dist) {
        case SpatialDistribution::Specific_Address:
            return tr("Specific");
        case SpatialDistribution::Bit_Reversal:
            return tr("Bit-Reversal");
        case SpatialDistribution::Perfect_Shuffle:
            return tr("Perfect Shuffle");
        case SpatialDistribution::Butterfly:
            return tr("Butterfly");
        case SpatialDistribution::Matrix_Transpose:
            return tr("Matrix Transpose");
        case SpatialDistribution::Complement:
            return tr("Complement");
        case SpatialDistribution::Uniform:
            return tr("Uniform");
        case SpatialDistribution::Non_Uniform:
            return tr("Non-Uniform");
        case SpatialDistribution::Local:
            return tr("Local");
    }
    return QString();
}

QString TrafficParameters::getSpatialDistributionName(int index) const {
    SpatialDistribution::Distribution dist = static_cast<SpatialDistribution::Distribution>(index);
    return this->getSpatialDistributionName( dist );
}

QString TrafficParameters::getSpatialDistributionName() const {
    return this->getSpatialDistributionName(this->distribution);
}

int TrafficParameters::indexOfSpatialDistribution(QString name) const {

    if( name.compare(tr("Specific")) == 0 ) {
        return 0;
    }
    if( name.compare(tr("Bit-Reversal")) == 0 ) {
        return 1;
    }
    if( name.compare(tr("Perfect Shuffle")) == 0 ) {
        return 2;
    }
    if( name.compare(tr("Butterfly")) == 0 ) {
        return 3;
    }
    if( name.compare(tr("Matrix Transpose")) == 0 ) {
        return 4;
    }
    if( name.compare(tr("Complement")) == 0 ) {
        return 5;
    }
    if( name.compare(tr("Uniform")) == 0 ) {
        return 6;
    }
    if( name.compare(tr("Non-Uniform")) == 0 ) {
        return 7;
    }
    if( name.compare(tr("Local")) == 0 ) {
        return 8;
    }

    return -1; // Not found
}

bool TrafficParameters::isValidForSystem(unsigned int numElements) {

    if( source < numElements && destination < numElements && source != destination) {
        return true;
    }

    return false;
}

QString TrafficParameters::getTrafficClassName(int index) const {
    switch (index) {
        case 0:
            return tr("RT0 - Signalling");
        case 1:
            return tr("RT1 - Audio/Video");
        case 2:
            return tr("nRT0 - Read/Write");
        case 3:
            return tr("nRT1 - Block Transfers");
        default:
            break;
    }
    return QString();

}

QString TrafficParameters::getTrafficClassName() const {
    return this->getTrafficClassName(this->trafficClass);
}

int TrafficParameters::indexOfTrafficClass(QString name) const {

    if( name.compare(tr("RT0 - Signalling")) == 0 ) {
        return 0;
    }
    if( name.compare(tr("RT1 - Audio/Video")) == 0 ) {
        return 1;
    }
    if( name.compare(tr("nRT0 - Read/Write")) == 0 ) {
        return 2;
    }
    if( name.compare(tr("nRT1 - Block Transfers")) == 0 ) {
        return 3;
    }
    return -1; // Not found
}

QString TrafficParameters::getInjectionTypeName(int index) const {

    switch (index) {
        case 0:
            return tr("Constant");
        case 1:
            return tr("Variable idle time - Fix message size");
        case 2:
            return tr("Variable message size - Fix idle time");
        case 3:
            return tr("Variable message size - Fix message inter-arrival");
        case 4:
            return tr("Variable message inter-arrival - Fix message size");
        case 5:
            return tr("Variable burst size - Fix message inter-arrival");
        default:break;
    }
    return QString();
}

QString TrafficParameters::getInjectionTypeName() const {
    return this->getInjectionTypeName(this->injectionType);
}

int TrafficParameters::indexOfInjectionType(QString name) const {

    if( name.compare(tr("Constant")) == 0 ) {
        return 0;
    }
    if( name.compare(tr("Variable idle time - Fix message size")) == 0 ) {
        return 1;
    }
    if( name.compare(tr("Variable message size - Fix idle time")) == 0 ) {
        return 2;
    }
    if( name.compare(tr("Variable message size - Fix message inter-arrival")) == 0 ) {
        return 3;
    }
    if( name.compare(tr("Variable message inter-arrival - Fix message size")) == 0 ) {
        return 4;
    }
    if( name.compare(tr("Variable burst size - Fix message inter-arrival")) == 0 ) {
        return 5;
    }
    return -1; // Not found
}

QString TrafficParameters::getSwitchingTechniqueName(int index) const {
    switch (index) {
        case 0:
            return tr("Wormhole Switching");
        case 1:
            return tr("Circuit Switching");
        default:break;
    }
    return QString();
}

QString TrafficParameters::getSwitchingTechniqueName() const {
    return this->getSwitchingTechniqueName(this->switchingTechnique);
}

int TrafficParameters::indexOfSwitchingTechnique(QString name) const {
    if( name.compare(tr("Wormhole Switching")) == 0 ) {
        return 0;
    }
    if( name.compare(tr("Circuit Switching")) == 0 ) {
        return 1;
    }
    return -1; // Not found
}

QString TrafficParameters::getProbabilityFunctionName(int index) const {
    switch(index) {
        case 0:
            return tr("Normal");
        case 1:
            return tr("Exponential");
        case 2:
            return tr("Pareto");
        default:break;
    }
    return QString();
}

QString TrafficParameters::getProbabilityFunctionName() const {
    return this->getProbabilityFunctionName(this->probabilityFunction);
}

int TrafficParameters::indexOfProbabilityFunction(QString name) const {
    if( name.compare(tr("Normal")) == 0 ) {
        return 0;
    }
    if( name.compare(tr("Exponential")) == 0 ) {
        return 1;
    }
    if( name.compare(tr("Pareto")) == 0 ) {
        return 2;
    }
    return -1; // Not found

}
