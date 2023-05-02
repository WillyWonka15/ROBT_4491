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
#define GPIO_PICKUP_BLOB 61
#define GPIO_DROP_BLOB 123
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

#endif /* CONTROLLERTOPLC_H_ */
