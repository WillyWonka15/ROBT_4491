//************************************************************************
//
// FILE:   controllerToPlc
//
// TITLE:  source file to configure pin to communicate to PLC
//
// using function from ti library for the ePWM module
//
//
//************************************************************************
#include "controllerToPlc.h"
#include "driverlib.h"
#include "device.h"
//*************************************************************************
// Function: communicationInit
// - initialize the pins needed for communication with the PLC
// Arguments:
//
// return: none
// Author: Will Nguyen
// Date: April 28th, 2023
// Modified: April 28th, 2023
//***********************************************************************
void communicationInit()
{
    //pickup blob o/p
    GPIO_setMasterCore(GPIO_PICKUP_BLOB, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_61_GPIO61);
    GPIO_setDirectionMode(GPIO_PICKUP_BLOB, GPIO_DIR_MODE_OUT);

    //drop blob o/p
    GPIO_setMasterCore(GPIO_DROP_BLOB, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_123_GPIO123);
    GPIO_setDirectionMode(GPIO_DROP_BLOB, GPIO_DIR_MODE_OUT);

}
