#ifndef REPORTREADER_H
#define REPORTREADER_H

#include <list>

class DataReport;

class ReportReader {

public:
    ReportReader();
    std::list<DataReport *>* readLatencyDistributionReport(const char *file);
    std::list<DataReport *>* readReport(const char *file);


};

#endif // REPORTREADER_H
