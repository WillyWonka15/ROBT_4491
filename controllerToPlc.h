//************************************************************************
//
// FILE:   controllerToPlc
//
// TITLE:  header file to configure pin to communicate to PLC
//
// using function from ti library for the ePWM module
//
//
//************************************************************************
#ifndef CONTROLLERTOPLC_H_
#define CONTROLLERTOPLC_H_

#include "F2837xD_device.h"
//*************************************************************************
//PRE-DEFINE CONSTANT
#define GPIO_CONTROL_PLC 61
#define GPIO_PLC_DONE 123
#define PLC_DONE C28X_BIT5
//MACRO
#define GET_BLOCK  GpioDataRegs.GPBCLEAR.bit.GPIO61 = 1
#define RELEASE_BLOCK  GpioDataRegs.GPBSET.bit.GPIO61 = 1
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
void communicationInit();

//*************************************************************************
// Function:plcDoneISR
// - interrupt handler for plc communicate to microcontroller
//
// Arguments:
//
//
// return: none
// Author: Will Nguyen
// Date: April 24th, 2023
// Modified: may 7th, 2023
//************************************************************************
__interrupt void plcDoneISR(void);

#endif /* CONTROLLERTOPLC_H_ */
