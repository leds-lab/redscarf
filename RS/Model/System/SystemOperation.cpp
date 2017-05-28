#include "Model/System/SystemOperation.h"

#include "Model/System/Defines.h"

SystemOperation::SystemOperation()
{
    this->stopOption     = DefaultValuesSystem::DEFAULT_STOP_OPTION     ; // Stop simulation option
    this->stopTime_cycles= DefaultValuesSystem::DEFAULT_STOP_TIME_CYCLES; // Time to stop simulation in cycles
    this->stopTime_ns    = DefaultValuesSystem::DEFAULT_STOP_TIME_NS    ; // Time to stop simulation in ns
    this->tClk           = DefaultValuesSystem::DEFAULT_TCLK            ; // Clock cycle
    this->vcdOption      = DefaultValuesSystem::DEFAULT_VCD_OPTION      ; // VCD generation option

    this->fClkFirst    = DefaultValuesSystem::DEFAULT_FCLK_FIRST;
    this->fClkLast     = DefaultValuesSystem::DEFAULT_FCLK_LAST;
    this->fClkStep     = DefaultValuesSystem::DEFAULT_FCLK_STEP_VALUE;
    this->fClkStepType = DefaultValuesSystem::DEFAULT_FCLK_STEP_TYPE;
}
