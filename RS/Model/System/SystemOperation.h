#ifndef SYSTEMOPERATION_H
#define SYSTEMOPERATION_H


class SystemOperation {
public:
    SystemOperation();

//private:
    unsigned int        stopOption;                 // Stop simulation option
    unsigned long int   stopTime_cycles;            // Time to stop simulation in cycles
    unsigned long int   stopTime_ns;                // Time to stop simulation in ns
    float               tClk;                       // Clock cycle
    unsigned int        vcdOption;                  // VCD generation option

    float               channelBandwidth;

    float               fClkFirst;
    float               fClkLast;
    float               fClkStep;
    unsigned int        fClkStepType;

};

#endif // SYSTEMOPERATION_H
