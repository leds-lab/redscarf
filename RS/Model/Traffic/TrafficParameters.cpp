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
    int i = static_cast<int>(dist);
    return getSpatialDistributionName(i);
}

QString TrafficParameters::getSpatialDistributionName(int index) const {
    QStringList dists = availableSpatialDistributions();
    return dists.at(index);
}

QString TrafficParameters::getSpatialDistributionName() const {
    return this->getSpatialDistributionName(this->distribution);
}

int TrafficParameters::indexOfSpatialDistribution(QString name) const {
    QStringList dists = availableSpatialDistributions();
    return dists.indexOf(name);
}

bool TrafficParameters::isValidForSystem(unsigned int numElements) {

    if( source < numElements && destination < numElements && source != destination) {
        return true;
    }

    return false;
}

QString TrafficParameters::getTrafficClassName(int index) const {
    QStringList classes = availableTrafficClasses();
    return classes.at(index);
}

QString TrafficParameters::getTrafficClassName() const {
    return this->getTrafficClassName(this->trafficClass);
}

int TrafficParameters::indexOfTrafficClass(QString name) const {
    QStringList classes = availableTrafficClasses();
    return classes.indexOf(name);
}

QString TrafficParameters::getInjectionTypeName(int index) const {
    QStringList types = availableInjectionTypes();
    return types.at(index);
}

QString TrafficParameters::getInjectionTypeName() const {
    return this->getInjectionTypeName(this->injectionType);
}

int TrafficParameters::indexOfInjectionType(QString name) const {
    QStringList types = availableInjectionTypes();
    return types.indexOf(name);
}

QString TrafficParameters::getSwitchingTechniqueName(int index) const {
    QStringList techs = availableSwitchingTechniques();
    return techs.at(index);
}

QString TrafficParameters::getSwitchingTechniqueName() const {
    return this->getSwitchingTechniqueName(this->switchingTechnique);
}

int TrafficParameters::indexOfSwitchingTechnique(QString name) const {
    QStringList techs = availableSwitchingTechniques();
    return techs.indexOf(name);
}

QString TrafficParameters::getProbabilityFunctionName(int index) const {
    QStringList functions = availableProbabilityFunctions();
    return functions.at(index);
}

QString TrafficParameters::getProbabilityFunctionName() const {
    return this->getProbabilityFunctionName(this->probabilityFunction);
}

int TrafficParameters::indexOfProbabilityFunction(QString name) const {
    QStringList functions = availableProbabilityFunctions();
    return functions.indexOf(name);
}

QStringList TrafficParameters::availableSpatialDistributions() {
    QStringList distributions;

    distributions << tr("Specific")
                  << tr("Bit-Reversal")
                  << tr("Perfect Shuffle")
                  << tr("Butterfly")
                  << tr("Matrix Transpose")
                  << tr("Complement")
                  << tr("Uniform")
                  << tr("Non-Uniform")
                  << tr("Local");

    return distributions;
}

QStringList TrafficParameters::availableTrafficClasses() {
    QStringList classes;
    classes << tr("RT0 - Signalling")
            << tr("RT1 - Audio/Video")
            << tr("nRT0 - Read/Write")
            << tr("nRT1 - Block Transfers");
    return classes;
}

QStringList TrafficParameters::availableInjectionTypes() {

    QStringList types;
    types << tr("Constant")
          << tr("Variable idle time - Fix message size")
          << tr("Variable message size - Fix idle time")
          << tr("Variable message size - Fix message interval")
          << tr("Variable message interval - Fix message size")
          << tr("Variable burst size - Fix message interval");
    return types;
}

QStringList TrafficParameters::availableSwitchingTechniques() {
    QStringList techs;
    techs << tr("Wormhole Switching")
          << tr("Circuit Switching");
    return techs;
}

QStringList TrafficParameters::availableProbabilityFunctions() {
    QStringList functions;
    functions << tr("Normal")
              << tr("Exponential")
              << tr("Pareto");
    return functions;
}
