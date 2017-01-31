#ifndef __NEWTRAFFICANALYZER_H__
#define __NEWTRAFFICANALYZER_H__

#include "Model/Analysis/TrafficAnalysis.h"

class NewTrafficAnalyzer : public TrafficAnalysis {

protected:
    unsigned long long simulatedCycles;

    StatusAnalysis batchMethod();

public:
    NewTrafficAnalyzer(unsigned short numElements,unsigned short dataWidth,
                       float lower,float upper,float fClk,
                       unsigned short fifoOutDepth,
                       unsigned short flowControlType,
                       const char* workDir, const char* resultDir);

    StatusAnalysis makeAnalysis();

    StatusAnalysis readLogsFiles();

    ~NewTrafficAnalyzer();
};

#endif // NEWTRAFFICANALYZER_H
