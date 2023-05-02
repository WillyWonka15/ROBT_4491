//************************************************************************
//
// FILE:   motorDriver.h
//
// TITLE:  header file for implementing motor driver VNH7070AS
//
//
//
//************************************************************************
#ifndef MOTORDRIVER_H_
#define MOTORDRIVER_H_

#include "F2837xD_device.h"
//*************************************************************************
//DIRECTION PIN
#define GPIO_X_DIR_CCW 66
#define GPIO_X_DIR_CW 131
#define GPIO_X_HOME 130

#define GPIO_Y_DIR_CCW 63
#define GPIO_Y_DIR_CW 64
#define GPIO_Y_HOME 26
//*************************************************************************
//MACRO
#define CCW_DIR_X GpioDataRegs.GPCSET.bit.GPIO66 = 1; GpioDataRegs.GPECLEAR.bit.GPIO131 = 1
#define CW_DIR_X GpioDataRegs.GPCCLEAR.bit.GPIO66 = 1; GpioDataRegs.GPESET.bit.GPIO131 = 1
#define CCW_DIR_Y GpioDataRegs.GPBSET.bit.GPIO63 = 1; GpioDataRegs.GPCCLEAR.bit.GPIO64 = 1
#define CW_DIR_Y GpioDataRegs.GPBCLEAR.bit.GPIO63 = 1; GpioDataRegs.GPCSET.bit.GPIO64 = 1
#define HOME_DIR_X CW_DIR_X
#define HOME_DIR_Y CCW_DIR_Y
#define FORWARD_DIR_X CCW_DIR_X
#define FORWARD_DIR_Y CW_DIR_Y
//*************************************************************************
//DEFINE_FLAG
#define X_HOME_FLAG C28X_BIT0
#define Y_HOME_FLAG C28X_BIT1
//*************************************************************************

//*************************************************************************
// Function: motorDriverInit
// - initialize pwm module and pins needed for both motor driver, x and y direction
//
// Arguments:
//
//
// return: none
// Author: Will Nguyen
// Date: April 13th, 2023
// Modified: April 13th, 2023
//************************************************************************
void motorDriverInit();

//*************************************************************************
// Function:xHomeISR
// - interrupt handler for x axis reach home position
//
// Arguments:
//
//
// return: none
// Author: Will Nguyen
// Date: April 24th, 2023
// Modified: April 24th, 2023
//************************************************************************
__interrupt void xHomeISR(void);

//*************************************************************************
// Function:yHomeISR
// - interrupt handler for y axis reach home position
//
// Arguments:
//
//
// return: none
// Author: Will Nguyen
// Date: April 24th, 2023
// Modified: April 24th, 2023
//************************************************************************
__interrupt void yHomeISR(void);

#endif /* MOTORDRIVER_H_ */
