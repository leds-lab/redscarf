#include "SystemExecution.h"

SystemExecution::SystemExecution()
    : fClk(0.0f)
{}

SystemExecution::SystemExecution(SystemConfiguration conf,
                                 Experiment experiment, float
                                 operationFrequency,
                                 QString folder)
    : conf(conf),experiment(experiment),fClk(operationFrequency),folder(folder)
{}

