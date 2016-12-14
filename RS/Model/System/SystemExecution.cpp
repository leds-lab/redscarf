#include "SystemExecution.h"

SystemExecution::SystemExecution()
    : fClk(0.0f),vcdOption(false)
{}

SystemExecution::SystemExecution(SystemConfiguration conf,
                                 Experiment experiment, float
                                 operationFrequency,
                                 QString folder,
                                 bool vcdOption)
    : conf(conf),experiment(experiment),fClk(operationFrequency),folder(folder),vcdOption(vcdOption)
{}

