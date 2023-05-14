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
    GPIO_setMasterCore(GPIO_CONTROL_PLC, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_61_GPIO61);
    GPIO_setDirectionMode(GPIO_CONTROL_PLC, GPIO_DIR_MODE_OUT);
    //bring to high
    GpioDataRegs.GPBSET.bit.GPIO61 = 1;


    //drop blob o/p
    GPIO_setMasterCore(GPIO_PLC_DONE, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_123_GPIO123);
    GPIO_setDirectionMode(GPIO_PLC_DONE, GPIO_DIR_MODE_IN);

    //register xint 1
    /*Interrupt_register(INT_XINT2, &plcDoneISR);
    GPIO_setInterruptPin(GPIO_PLC_DONE, GPIO_INT_XINT2);
    GPIO_setInterruptType(GPIO_INT_XINT2, GPIO_INT_TYPE_FALLING_EDGE);
    GPIO_enableInterrupt(GPIO_INT_XINT2);
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP1);
    Interrupt_enable(INT_XINT2);*/
}
