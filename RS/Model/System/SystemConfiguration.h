#ifndef __SYSTEMCONFIGURATION_H__
#define __SYSTEMCONFIGURATION_H__

#include "Model/System/SystemParameters.h"
#include "Model/Traffic/TrafficParameters.h"

#include <QList>

class SystemConfiguration {
private:
    SystemParameters sysConf;
    QList<TrafficParameters> trafficConf;

public:
    SystemConfiguration();
    SystemConfiguration(SystemParameters sp, QList<TrafficParameters> traffic);

    inline SystemParameters getSystemParameters() const { return this->sysConf; }
    inline QList<TrafficParameters> getTrafficConfiguration() const { return this->trafficConf; }

    void addTrafficConfiguration(TrafficParameters tp);

    inline void setSystemParameters(SystemParameters sp) { this->sysConf = sp; }
    inline void setTrafficConfiguration(QList<TrafficParameters> trafficConf) { this->trafficConf = trafficConf; }

    bool isValid() const;
    bool hasTraffic() const;
};

#endif // __SYSTEMCONFIGURATION_H__
