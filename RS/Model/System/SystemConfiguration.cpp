#include "SystemConfiguration.h"

SystemConfiguration::SystemConfiguration() {}

SystemConfiguration::SystemConfiguration(SystemParameters sp, QList<TrafficParameters> traffic)
    : sysConf(sp), trafficConf(traffic)
{}

bool SystemConfiguration::isValid() const {
    if( this->sysConf.getNumberElements() > 0 ) {
        return true;
    }
    return false;
}

bool SystemConfiguration::hasTraffic() const {
    return !trafficConf.isEmpty();
}

void SystemConfiguration::addTrafficConfiguration(TrafficParameters tp) {
    this->trafficConf.append(tp);
}
