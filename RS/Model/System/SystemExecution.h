#ifndef __SYSTEMEXECUTION_H__
#define __SYSTEMEXECUTION_H__

#include "Model/System/SystemConfiguration.h"
#include "Model/System/Experiment.h"

class SystemExecution {
private:
    SystemConfiguration conf;   // System Configuration of execution
    Experiment experiment;      // Experiment of execution
    float fClk;                 // Operation frequency of execution
    QString folder;             // Folder of execution
    bool vcdOption;             // If vcd generation is enabled

public:
    SystemExecution();
    SystemExecution(SystemConfiguration conf,
                    Experiment experiment,
                    float operationFrequency,
                    QString folder,
                    bool vcdOption);

    inline void setSystemConfiguration(SystemConfiguration sysConf) { this->conf = sysConf; }
    inline void setExperiment(Experiment experiment) { this->experiment = experiment; }
    inline void setOperationFrequency(float op) { this->fClk = op; }
    inline void setWorkFolder(QString folder) { this->folder = folder; }
    inline void setVcdOption(bool option) { this->vcdOption = option; }

    inline SystemConfiguration getSystemConfiguration() const { return this->conf; }
    inline Experiment getExperiment() const { return this->experiment; }
    inline float getOperationFrequency() const { return this->fClk; }
    inline QString getWorkFolder() const { return this->folder; }
    inline bool isVcdOptionEnabled() const { return vcdOption; }
};

#endif // __SYSTEMEXECUTION_H__