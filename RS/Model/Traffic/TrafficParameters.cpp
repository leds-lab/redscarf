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

    QString formatted;

    switch (this->distribution) {
        case SpatialDistribution::Specific_Address:
            formatted = tr("Specific");
            break;
        case SpatialDistribution::Bit_Reversal:
            formatted = tr("Bit-Reversal");
            break;
        case SpatialDistribution::Perfect_Shuffle:
            formatted = tr("Perfect Shuffle");
            break;
        case SpatialDistribution::Butterfly:
            formatted = tr("Butterfly");
            break;
        case SpatialDistribution::Matrix_Transpose:
            formatted = tr("Local");
            break;
        case SpatialDistribution::Complement:
            formatted = tr("Complement");
            break;
        case SpatialDistribution::Uniform:
            formatted = tr("Uniform");
            break;
        case SpatialDistribution::Non_Uniform:
            formatted = tr("Non-Uniform");
            break;
        case SpatialDistribution::Local:
            formatted = tr("Local");
            break;
    }

    formatted += tr(" - from %1 to %2")
            .arg(source)
            .arg(destination);

    if( deadline > 0 ) {
        formatted += tr(" - Deadline: %1").arg(deadline);
    }

    formatted += tr(" - Req. BW(Mbps): %1").arg(requiredBandwidth);

    return formatted;
}
